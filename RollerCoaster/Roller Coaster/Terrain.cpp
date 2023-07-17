#include "Terrain.h"
#include<iostream>

#define manyface 500.0
#define watersize 1600.0
Terrain::Terrain()
{
}
void Terrain::DimensionTransformation(GLfloat source[], GLfloat target[][4])
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

int color_flag = 0;
void Terrain::Paint(GLfloat* ProjectionMatrix, GLfloat* ModelViewMatrix)
{
	GLfloat P[4][4];
	GLfloat MV[4][4];
	DimensionTransformation(ProjectionMatrix, P);
	DimensionTransformation(ModelViewMatrix, MV);
	//Bind the shader we want to draw with
	shaderProgram->bind();
	//Bind the VAO we want to draw
	vao.bind();
	/*if (clock() % 10 == 0)
	{
		if (color_flag == 0)
		{
			times = times + 0.01;
		}
		if (color_flag == 1)
		{
			times = times - 0.01;
		}
		if (times == 1)
		{
			color_flag = 1;
		}
		if (times == 0)
		{
			color_flag = 0;
		}
	}*/
	glEnable(GL_BLEND);
	//pass projection matrix to shader
	shaderProgram->setUniformValue("ProjectionMatrix", P);
	//pass modelview matrix to shader
	shaderProgram->setUniformValue("ModelViewMatrix", MV);

	shaderProgram->setUniformValue("times",0);
	// Bind the buffer so that it is the current active buffer.
	vvbo.bind();
	// Enable Attribute 0
	shaderProgram->enableAttributeArray(0);
	// Set Attribute 0 to be position
	shaderProgram->setAttributeArray(0, GL_FLOAT, 0, 3, NULL);
	//unbind buffer
	vvbo.release();

	//Draw a triangle with 3 indices starting from the 0th index
	glDrawArrays(GL_TRIANGLES, 0, position.size());
	//Disable Attribute 0&1
	shaderProgram->disableAttributeArray(0);
	//shaderProgram->disableAttributeArray(1);

	//unbind vao
	vao.release();
	//unbind vao
	shaderProgram->release();
	glDisable(GL_BLEND);
}
void Terrain::Init()
{
	InitShader("./Shader/Terrain.vs", "./Shader/Terrain.fs");
	InitVAO();
	InitVBO();
}
void Terrain::InitVAO()
{
	// Create Vertex Array Object
	vao.create();
	// Bind the VAO so it is the current active VAO
	vao.bind();
}
void Terrain::InitVBO()
{
	GLfloat hight = 2.0;
	//Set each vertex's position
	for (int i = -manyface / 2; i < (manyface / 2) - 1; i++)
	{
		for (int j = -manyface / 2; j < (manyface / 2) - 1; j++)
		{
			position << QVector3D((float)(watersize / manyface)*i, hight, (float)(watersize / manyface)*j) << QVector3D((float)(watersize / manyface)*(i + 1), hight, (float)(watersize / manyface)*j) << QVector3D((float)(watersize / manyface)*(i + 1), hight, (float)(watersize / manyface)*(j + 1));
			position << QVector3D((float)(watersize / manyface)*i, hight, (float)(watersize / manyface)*j) << QVector3D((float)(watersize / manyface)*i, hight, (float)(watersize / manyface)*(j + 1)) << QVector3D((float)(watersize / manyface)*(i + 1), hight, (float)(watersize / manyface)*(j + 1));
		}
	}
	// Create Buffer for position
	vvbo.create();
	// Bind the buffer so that it is the current active buffer.
	vvbo.bind();
	// Since we will never change the data that we are about to pass the Buffer, we will say that the Usage Pattern is StaticDraw
	vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// Allocate and initialize the information
	vvbo.allocate(position.constData(), position.size() * sizeof(QVector3D));

}
void Terrain::InitShader(QString vertexShaderPath, QString fragmentShaderPath)
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
void Terrain::Begin()
{
	//Bind the shader we want to draw with
	shaderProgram->bind();
	//Bind the VAO we want to draw
	vao.bind();
}