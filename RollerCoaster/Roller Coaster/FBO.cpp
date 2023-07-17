#include "FBO.h"
#include<string>
#include<vector>
#include<iostream>

using namespace std;

void FBO::DimensionTransformation(GLfloat source[], GLfloat target[][4])
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
void FBO::Begin()
{
	//Bind the shader we want to draw with
	shaderProgram->bind();
	//Bind the VAO we want to draw
	vao.bind();
}
float timess = 0;
int color_flags = 0;
void FBO::Paint(GLfloat* ProjectionMatrix, GLfloat* ModelViewMatrix)
{
	//GLfloat P[4][4];
	//GLfloat MV[4][4];
	//DimensionTransformation(ProjectionMatrix, P);
	//DimensionTransformation(ModelViewMatrix, MV);
	////pass projection matrix to shader
	//shaderProgram->setUniformValue("ProjectionMatrix", P);
	////pass modelview matrix to shader
	//shaderProgram->setUniformValue("ModelViewMatrix", MV);

	if (clock() % 10 == 0)
	{
		if (color_flags == 0)
		{
			timess = timess + 0.05;
		}
		if (timess > 100000)
		{
			timess = 0;
		}
	}

	shaderProgram->setUniformValue("time", timess);

	nvbo.bind();
	// Enable Attribute 0
	shaderProgram->enableAttributeArray(0);
	// Set Attribute 0 to be position
	shaderProgram->setAttributeArray(0, GL_FLOAT, 0, 3, NULL);
	//unbind buffer
	nvbo.release();


	uvbo.bind();
	// Enable Attribute 0
	shaderProgram->enableAttributeArray(1);
	// Set Attribute 0 to be position
	shaderProgram->setAttributeArray(1, GL_FLOAT, 0, 2, NULL);
	//unbind buffer
	uvbo.release();
	//Draw triangles with 4 indices starting from the 0th index


	//glDrawElements(GL_TRIANGLES, m.m_pointIndices.size(), GL_UNSIGNED_INT, m.m_pointIndices.data());
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	shaderProgram->disableAttributeArray(0);
	shaderProgram->disableAttributeArray(1);
	//unbind vao
	vao.release();
	//unbind vao
	shaderProgram->release();
}
void FBO::End()
{
	vao.release();
	shaderProgram->release();
}

FBO::FBO()
{
}

void FBO::Init()
{
	Point3d idntity = { 0,0,0 };
	InitShader("./Shader/FBO.vs", "./Shader/FBO.fs");
	InitVAO();
	InitVBO();
}
void FBO::InitVAO()
{
	// Create Vertex Array Object
	vao.create();
	// Bind the VAO so it is the current active VAO
	vao.bind();
}
void FBO::InitVBO()
{
	initializeOpenGLFunctions();
	glGenFramebuffers(1, &FramebufferName);
	glGenTextures(1, &renderedTexture);
	// "Bind" the newly created texture : all future texture functions will modify this texture 
	
	glGenRenderbuffers(1, &depthrenderbuffer);
	

	//Set each vertex's color
	vertices << QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(1.0f, -1.0f, 0.0f)
		<< QVector3D(1.0f, 1.0f, 0.0f)

		<< QVector3D(-1.0f, -1.0f, 0.0f)
		<< QVector3D(1.0f, 1.0f, 0.0f)
		<< QVector3D(-1.0f, 1.0f, 0.0f);
	// Create Buffer for color
	nvbo.create();
	// Bind the buffer so that it is the current active buffer.
	nvbo.bind();
	// Since we will never change the data that we are about to pass the Buffer, we will say that the Usage Pattern is StaticDraw
	nvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// Allocate and initialize the information
	nvbo.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));

	UV << QVector2D(0.0f,0.0f)
		<< QVector2D(1.0f, 0.0f)
		<< QVector2D(1.0f, 1.0f)

		<< QVector2D(0.0f, 0.0f)
		<< QVector2D(1.0f, 1.0f)
		<< QVector2D(0.0f, 1.0f);
	// Create Buffer for color
	uvbo.create();
	// Bind the buffer so that it is the current active buffer.
	uvbo.bind();
	// Since we will never change the data that we are about to pass the Buffer, we will say that the Usage Pattern is StaticDraw
	uvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// Allocate and initialize the information
	uvbo.allocate(UV.constData(), UV.size() * sizeof(QVector3D));

}
void FBO::InitShader(QString vertexShaderPath, QString fragmentShaderPath)
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
