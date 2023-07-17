#pragma once

//#include "Include/TinyOBJ/tiny_obj_loader.h"

#include "Include/Assimp/Importer.hpp"
#include "Include/Assimp/scene.h"
#include "Include/Assimp/postprocess.h"
//sdcsdsdsdcsdcsdcsd
#include<iostream>
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
#include <QFileInfo>
#include <QOpenGLTexture>
#include <QImage>
#include <QDebug>
#include <QString>
#include <vector>
#include "mesh.h"
using namespace std;
class MyModel : protected QOpenGLFunctions_4_3_Core
{
public:
	void Init(string const &path, QString BaseMap, glm::vec3 pos, float scale) {
		initializeOpenGLFunctions();
		this->initPos = pos;
		this->initScale = scale;
		loadModel(path);
		myTexture = new QOpenGLTexture(QImage(BaseMap));
		SpecF = 0;
		NormF = 0;
		isToon = 0;
	}
	void Init(string const &path, QString str, QString str2, glm::vec3 pos, float scale) {
		initializeOpenGLFunctions();
		this->initPos = pos;
		this->initScale = scale;
		loadModel(path);
		myTexture = new QOpenGLTexture(QImage(str));
		NTexture = new QOpenGLTexture(QImage(str2));
		SpecF = 0;
		NormF = 1;
		isToon = 0;
	}
	void Init(string const &path, QString BaseMap, QString NormalMap, QString SpecMap, glm::vec3 pos, float scale) {
		initializeOpenGLFunctions();
		this->initPos = pos;
		this->initScale = scale;
		loadModel(path);
		myTexture = new QOpenGLTexture(QImage(BaseMap));
		NTexture = new QOpenGLTexture(QImage(NormalMap));
		SpecTexture = new QOpenGLTexture(QImage(SpecMap));
		SpecF = 1;
		NormF = 1;
		isToon = 0;
	}
	void Paint(GLfloat* ProjectionMatrix, GLfloat* ModelViewMatrix, GLfloat MoveMatrix[16],GLfloat light[3],int istoon)
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].shaderProgram->setUniformValue("text", 0);
			meshes[i].Paint(ProjectionMatrix, ModelViewMatrix, MoveMatrix, light,istoon);
		}
	}
public:
	glm::vec3 initPos;
	float initScale;
	QOpenGLTexture* myTexture;
	QOpenGLTexture* NTexture;
	QOpenGLTexture* SpecTexture;
	bool SpecF;
	bool NormF;
	bool isToon;
	vector<Mesh> meshes;

	void loadModel(string const &path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		//else cout << "Model Load Success\n";
		// retrieve the directory path of the filepath
		//directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// data to fill
		vector<Vertex> vertices;
		vector<unsigned int> indices;

		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
							  // positions
			vector.x = mesh->mVertices[i].x * initScale + initPos.x;
			vector.y = mesh->mVertices[i].y * initScale + initPos.y;
			vector.z = mesh->mVertices[i].z * initScale + initPos.z;
			vertex.Position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			// tangent
			if (mesh->mTangents)
			{
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
			}
			else
			{
				vector = glm::vec3(0.0f, 0.0f, 0.0f);
			}

			vertex.Tangent = vector;
			// bitangent

			if (mesh->mBitangents)
			{
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
			}
			else
			{
				vector = glm::vec3(0.0f, 0.0f, 0.0f);
			}

			vertex.Bitangent = vector;
			vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		return Mesh(vertices, indices);
	}
};

