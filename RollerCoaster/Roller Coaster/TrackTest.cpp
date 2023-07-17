#include "TrackTest.h"
#include<iostream>
TrackTest::TrackTest()
{
}
void TrackTest::DimensionTransformation(GLfloat source[], GLfloat target[][4])
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

void TrackTest::Paint(GLfloat* ProjectionMatrix, GLfloat* ModelViewMatrix, Pnt3f controlP1, Pnt3f controlP2, Pnt3f controlP3, Pnt3f controlP4, Pnt3f orientP1, Pnt3f orientP2, Pnt3f orientP3, Pnt3f orientP4, int curveType,int in_or_out)
{

	GLfloat P[4][4];
	GLfloat MV[4][4];
	GLfloat GG[4][4];
	DimensionTransformation(ProjectionMatrix, P);
	DimensionTransformation(ModelViewMatrix, MV);

	//Bind the shader we want to draw with
	shaderProgram->bind();
	//Bind the VAO we want to draw
	vao.bind();

	//pass projection matrix to shader
	shaderProgram->setUniformValue("ProjectionMatrix", P);
	//pass modelview matrix to shader
	shaderProgram->setUniformValue("ModelViewMatrix", MV);
	shaderProgram->setUniformValue("controlP1", QVector3D(controlP1.x, controlP1.y, controlP1.z));
	shaderProgram->setUniformValue("controlP2", QVector3D(controlP2.x, controlP2.y, controlP2.z));
	shaderProgram->setUniformValue("controlP3", QVector3D(controlP3.x, controlP3.y, controlP3.z));
	shaderProgram->setUniformValue("controlP4", QVector3D(controlP4.x, controlP4.y, controlP4.z));
	shaderProgram->setUniformValue("orientP1", QVector3D(orientP1.x, orientP1.y, orientP1.z));
	shaderProgram->setUniformValue("orientP2", QVector3D(orientP2.x, orientP2.y, orientP2.z));
	shaderProgram->setUniformValue("orientP3", QVector3D(orientP3.x, orientP3.y, orientP3.z));
	shaderProgram->setUniformValue("orientP4", QVector3D(orientP4.x, orientP4.y, orientP4.z));
	shaderProgram->setUniformValue("curveType", curveType);
	shaderProgram->setUniformValue("in_or_out", in_or_out);


	// Bind the buffer so that it is the current active buffer.
	vvbo.bind();
	// Enable Attribute 0
	shaderProgram->enableAttributeArray(0);
	// Set Attribute 0 to be position
	shaderProgram->setAttributeArray(0, GL_FLOAT, 0, 3, NULL);
	//unbind buffer
	vvbo.release();
	glLineWidth(3);
	//Draw a triangle with 3 indices starting from the 0th index
	glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
	//Disable Attribute 0&1
	shaderProgram->disableAttributeArray(0);

	//unbind vao
	vao.release();
	//unbind vao
	shaderProgram->release();
}
void TrackTest::Init(int divide, CTrack* track)
{
	divide_line = divide;
	track_p = track;
	InitShader("./Shader/TrackTest.vs", "./Shader/TrackTest.fs");
	InitVAO();
	InitVBO();
}
void TrackTest::InitVAO()
{
	// Create Vertex Array Object

	vao.create();
	// Bind the VAO so it is the current active VAO
	vao.bind();
}
void TrackTest::InitVBO()
{
	vertices.clear();
	for (int i = 0; i <= divide_line; i++) 
	{
		vertices << QVector3D(i, divide_line, 0.0f);
	}
	//Set each vertex's position 
	/*for (int i = 0 ; i< track_p->points.size();i++)
	{
		Pnt3f cp_pos_p1 = track_p->points[i].pos;
		Pnt3f cp_pos_p2 = track_p->points[(i + 1) % track_p->points.size()].pos;
		float percent = 1.0f / divide_line;
		float t = 0.0f;
		for (int j = 0; j < divide_line; j++)
		{

			Pnt3f delta = (1.0f - t) *cp_pos_p1+ t*cp_pos_p2;
			vertices << QVector3D(delta.x,delta.y,delta.z);
			t+=percent;
		}
	}*/
	/*vertices
		<< QVector3D(track_p->points[0].pos.x, track_p->points[0].pos.y, track_p->points[0].pos.z)
		<< QVector3D(track_p->points[1].pos.x, track_p->points[1].pos.y, track_p->points[1].pos.z)
		<< QVector3D(track_p->points[2].pos.x, track_p->points[2].pos.y, track_p->points[2].pos.z)
		<< QVector3D(track_p->points[3].pos.x, track_p->points[3].pos.y, track_p->points[3].pos.z);*/
		// Create Buffer for position
	vvbo.create();
	// Bind the buffer so that it is the current active buffer.
	vvbo.bind();
	// Since we will never change the data that we are about to pass the Buffer, we will say that the Usage Pattern is StaticDraw
	vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// Allocate and initialize the information
	vvbo.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));



}
void TrackTest::InitShader(QString vertexShaderPath, QString fragmentShaderPath)
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