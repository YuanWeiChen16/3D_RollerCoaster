#include "Water.h"

Water::Water()
{
}
void Water::DimensionTransformation(GLfloat source[], GLfloat target[][4])
{
	//for uniform value, transfer 1 dimension to 2 dimension
	int i = 0;
	for (int j = 0; j < 4; j++)
		for (int k = 0; k < 4; k++)
		{
			target[j][k] = source[i];
			i++;
		}
}
float t = 0;
float lasttimes = 0;
float times = 0;
void Water::Paint(GLfloat* ProjectionMatrix, GLfloat* ModelViewMatrix)
{
	if (clock() - lasttimes > CLOCKS_PER_SEC / 60) {
		lasttimes = clock();
		//cout << times << endl;
		times += 1.0 / 60.0;

	}
	GLfloat P[4][4];
	GLfloat MV[4][4];

	DimensionTransformation(ProjectionMatrix, P);
	DimensionTransformation(ModelViewMatrix, MV);

	//Bind the shader we want to draw with
	shaderProgram->bind();
	//Bind the VAO we want to draw
	vao.bind();
	glEnable(GL_BLEND);
	glDepthMask(TRUE);
	shaderProgram->setUniformValue("waterHeight", -50);
	shaderProgram->setUniformValue("numWaves", 8);
	//shaderProgram->setUniformValue("envMap", 0);
	GLfloat amp[8];
	GLfloat wavel[8];
	GLfloat speeds[8];
	QVector2D dir[8] = { {0.03,0.04 },{-0.03,0.04 },{0.03,-0.04 },{0.05,0.02 },{0.03,0.05 },{-0.03,-0.04 },{-0.06,-0.07 },{0.02,-0.05 },
};
	for (int i = 0; i < 8; ++i) {
		float amplitude = 1.5f / (i + 1);

		amp[i] = amplitude;

		float wavelength = 4 * 3.14159 / (i + 1);

		wavel[i] = wavelength;
		float speed = 1.0f + 2 * i;

		speeds[i] = speed;

		float angle = (-3.14159 / 3.0) + ((double)rand() / (double)RAND_MAX)* ((3.14159 / 3.0) + (3.14159 / 3.0));
		//dir[i].setX(cos(angle));
		//dir[i].setY(sin(angle));
		//dir[i].setX(0.03);
		//dir[i].setY(0.04);

	}
	t += 0.0005;
	shaderProgram->setUniformValueArray("amplitude", amp, 8, 1);
	shaderProgram->setUniformValueArray("wavelength", wavel, 8, 1);
	shaderProgram->setUniformValueArray("speed", speeds, 8, 1);
	shaderProgram->setUniformValueArray("direction", dir, 8);
	shaderProgram->setUniformValue("time", t);
	shaderProgram->setUniformValue("eyePos", 0.0f, 10.0f, 0.0f);
	//pass projection matrix to shader

	shaderProgram->setUniformValue("proj_matrix", P);
	//pass modelview matrix to shader
	shaderProgram->setUniformValue("model_matrix", MV);
	shaderProgram->setUniformValue("model_matrixs", MV);
	//cout << "?" << endl;
	//shaderProgram->setUniformValue("deltaTime", deltaTime);

	// Bind the buffer so that it is the current active buffer.
	vvbo.bind();
	// Enable Attribute 0
	shaderProgram->enableAttributeArray(0);
	// Set Attribute 0 to be position
	shaderProgram->setAttributeArray(0, GL_FLOAT, 0, 3, NULL);
	//unbind buffer
	vvbo.release();


	glDrawArrays(GL_TRIANGLES, 0, vertices.size());



	shaderProgram->disableAttributeArray(0);


	//unbind vao
	vao.release();
	//unbind vao
	shaderProgram->release();


	glDisable(GL_BLEND);
}

void Water::Init()
{
	InitShader("./Shader/water.vs", "./Shader/water.fs");
	InitVAO();
	InitVBO();
}
void Water::InitVAO()
{
	// Create Vertex Array Object
	vao.create();
	// Bind the VAO so it is the current active VAO
	vao.bind();
}
void Water::InitVBO()
{
	//Set each vertex's position
	float xStart = -1000;
	float xEnd = 1000;
	float zStart = -1000;
	float zEnd = 1000;
	float xDelta = xEnd - xStart;
	float zDelta = zEnd - zStart;
	int divide =299.0;
	for (int i = 0; i < divide; i++) //x
	{
		for (int j = 0; j < divide; j++) //z
		{
			float x1 = xStart + (float)i*(xDelta / divide);
			float z1 = zStart + (float)j*(zDelta / divide);
			float x2 = xStart + (float)(i + 1)*(xDelta / divide);
			float z2 = zStart + (float)j*(zDelta / divide);
			float x3 = xStart + (float)i*(xDelta / divide);
			float z3 = zStart + (float)(j + 1)*(zDelta / divide);
			float x4 = xStart + (float)(i + 1)*(xDelta / divide);
			float z4 = zStart + (float)(j + 1)*(zDelta / divide);
			//cout << x1 << endl;
			vertices << QVector3D(x1, 0.0f, z1) << QVector3D(x2, 0.0f, z2) << QVector3D(x3, 0.0f, z3);
			vertices << QVector3D(x3, 0.0f, z3) << QVector3D(x2, 0.0f, z2) << QVector3D(x4, 0.0f, z4);
		}
	}

	// Create Buffer for position
	vvbo.create();
	// Bind the buffer so that it is the current active buffer.
	vvbo.bind();
	// Since we will never change the data that we are about to pass the Buffer, we will say that the Usage Pattern is StaticDraw
	vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// Allocate and initialize the information
	vvbo.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));

	//Set each vertex's color



}
void Water::InitShader(QString vertexShaderPath, QString fragmentShaderPath)
{
	// Create Shader
	shaderProgram = new QOpenGLShaderProgram();
	QFileInfo  vertexShaderFile(vertexShaderPath);
	if (vertexShaderFile.exists())
	{
		vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
		if (vertexShader->compileSourceFile(vertexShaderPath))
			shaderProgram->addShader(vertexShader);
		else
			qWarning() << "Vertex Shader Error " << vertexShader->log();
	}
	else
		qDebug() << vertexShaderFile.filePath() << " can't be found";

	QFileInfo  fragmentShaderFile(fragmentShaderPath);
	if (fragmentShaderFile.exists())
	{
		fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
		if (fragmentShader->compileSourceFile(fragmentShaderPath))
			shaderProgram->addShader(fragmentShader);
		else
			qWarning() << "fragment Shader Error " << fragmentShader->log();
	}
	else
		qDebug() << fragmentShaderFile.filePath() << " can't be found";
	shaderProgram->link();
}