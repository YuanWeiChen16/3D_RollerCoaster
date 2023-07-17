#pragma once
#define MESH_H



#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm/glm/glm.hpp>
#include "Utilities/ArcBallCam.H"
#include <QtGui/QOpenGLFunctions_4_3_Core>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLShaderProgram>
#include <QtOpenGL/QtOpenGL> 
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QString>
#include <QOpenGLTexture>
#include <QImage>

using namespace std;
struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

class Mesh
{
public:
	Mesh();
	Mesh(vector<Vertex> v, vector<unsigned int> i) {
		this->vertices = v;
		this->indices = i;
		setup();
	}
	Mesh(const Mesh& M) {
		this->vertices = M.vertices;
		this->indices = M.indices;
		setup();
	}
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	QOpenGLShaderProgram * shaderProgram;
	QOpenGLShader* vertexShader;
	QOpenGLShader* fragmentShader;
	QVector<QVector3D> Pos;
	QVector<QVector3D> normal;
	QVector<QVector2D> uvs;
	QVector<unsigned int> Indices;
	QOpenGLVertexArrayObject vao;
	QOpenGLBuffer vvbo;
	QOpenGLBuffer uvbo;
	QOpenGLBuffer nvbo;

	void setup() {
		InitShader("./Shader/MyModel.vs", "./Shader/MyModel.fs");
		InitVAO();
		InitVBO();
	}

	void InitVAO() {
		vao.create();
		vao.bind();
	}

	void InitVBO() {
		for (int j = 0; j < vertices.size(); j++) {
			Pos << QVector3D(vertices[j].Position.x, vertices[j].Position.y, vertices[j].Position.z);
			normal << QVector3D(vertices[j].Normal.x, vertices[j].Normal.y, vertices[j].Normal.z);
			uvs << QVector2D(vertices[j].TexCoords.x, vertices[j].TexCoords.y);
		}
		for (int k = 0; k < indices.size(); k++)
			Indices << indices[k];


#ifdef DEBUG
		cout << "positions : " << Pos.size() << endl;
		cout << "normal : " << normal.size() << endl;
		cout << "texcoords : " << uvs.size() << endl;
		cout << "Indices : " << Indices.size() << endl;
#endif
		vvbo.create();
		vvbo.bind();
		vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
		vvbo.allocate(Pos.constData(), Pos.size() * sizeof(QVector3D));

		nvbo.create();
		nvbo.bind();
		nvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
		nvbo.allocate(normal.constData(), normal.size() * sizeof(QVector3D));

		uvbo.create();
		uvbo.bind();
		uvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
		uvbo.allocate(uvs.constData(), uvs.size() * sizeof(QVector2D));
	}

	void InitShader(QString vertexShaderPath, QString fragmentShaderPath) {
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

	void DimensionTransformation(GLfloat source[], GLfloat target[][4]) {
		int i = 0;
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
			{
				target[j][k] = source[i];
				i++;
			}
	}

	void Begin() {
		shaderProgram->bind();
		vao.bind();
	}

	void Paint(GLfloat* ProjectionMatrix, GLfloat* ModelViewMatrix,GLfloat MoveMatrix[16], GLfloat light[3],int isToon)
	{
		Begin();
		GLfloat P[4][4];
		GLfloat MV[4][4];
		GLfloat M[4][4];
		DimensionTransformation(ProjectionMatrix, P);
		DimensionTransformation(ModelViewMatrix, MV);
		DimensionTransformation(MoveMatrix, M);
		shaderProgram->setUniformValue("ProjectionMatrix", P);
		shaderProgram->setUniformValue("ModelViewMatrix", MV);
		shaderProgram->setUniformValue("MoveMatrix", M);
		shaderProgram->setUniformValue("light_dir", light[0], light[1], light[2]);
		shaderProgram->setUniformValue("isToon", isToon);

		DimensionTransformation(ProjectionMatrix, P);
		DimensionTransformation(ModelViewMatrix, MV);

		shaderProgram->setUniformValue("ProjectionMatrix", P);
		shaderProgram->setUniformValue("ModelViewMatrix", MV);
		shaderProgram->setUniformValue("isToon", isToon);

		vvbo.bind();
		shaderProgram->enableAttributeArray(0);
		shaderProgram->setAttributeArray(0, GL_FLOAT, 0, 3, NULL);
		vvbo.release();

		nvbo.bind();
		shaderProgram->enableAttributeArray(1);
		shaderProgram->setAttributeArray(1, GL_FLOAT, 0, 3, NULL);
		nvbo.release();

		uvbo.bind();
		shaderProgram->enableAttributeArray(2);
		shaderProgram->setAttributeArray(2, GL_FLOAT, 0, 2, NULL);
		uvbo.release();

		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, Indices.constData());
		End();
	}

	void End() {
		shaderProgram->disableAttributeArray(0);
		shaderProgram->disableAttributeArray(1);
		shaderProgram->disableAttributeArray(2);
		vao.release();
		shaderProgram->release();
	}
};