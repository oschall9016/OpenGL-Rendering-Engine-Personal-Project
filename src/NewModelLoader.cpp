#include "NewModelLoader.h"
#include "NewMesh.h"
#include "Model.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <stb_image.h>

#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include <memory>

std::shared_ptr<Model> NewModelLoader::LoadModel(const std::string& path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); // look into the flags

	if (scene == nullptr)
	{
		std::cout << "Failed to Load Model: " << importer.GetErrorString() << "\n";
	}

	std::vector<NewMesh> meshes;

	TraverseAssimpScene(scene, meshes);

	std::shared_ptr<Model> model = make_shared<Model>(meshes);

	return model;

}

void NewModelLoader::TraverseAssimpScene(const aiScene* scene, std::vector<NewMesh>& meshes)
{
	
	std::stack<aiNode*> nodeStack;
	nodeStack.push(scene->mRootNode);

	while (!nodeStack.empty())
	{
		aiNode* curNode = nodeStack.top();
		nodeStack.pop();

		// process node
		for (unsigned int i = 0; i < curNode->mNumMeshes; i++)
		{
			unsigned int meshIndex = curNode->mMeshes[i];
			NewMesh tempMesh = CreateMesh(scene->mMeshes[meshIndex]);
			meshes.push_back(tempMesh);
		}

		for (unsigned int i = 0; i < curNode->mNumChildren; i++)
		{
			nodeStack.push(curNode->mChildren[i]);
		}

	}
}

NewMesh NewModelLoader::CreateMesh(aiMesh* mesh)
{
	std::vector<NewVertex> tempVertices;
	std::vector<unsigned int> tempIndices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		NewVertex tempVertex;
		tempVertex.position = glm::vec3
		(
			mesh->mVertices[i].x, 
			mesh->mVertices[i].y, 
			mesh->mVertices[i].z
		);

		tempVertices.push_back(tempVertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			tempIndices.push_back(face.mIndices[j]);
		}
	}

	return NewMesh(tempVertices, tempIndices);
}

std::shared_ptr<Texture> NewModelLoader::LoadTexture(const std::string& path)
{
	int width, height, nrChannels;
	unsigned char* data;
	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cout << "Failed to Load Texture" << "\n";
		return nullptr;
	}
	
	GLenum format;
	if (nrChannels == 1) format = GL_RED;
	else if (nrChannels == 3) format = GL_RGB;
	else if (nrChannels == 4) format = GL_RGBA;
	else
	{
		std::cout << "Unrecognized Channel Count" << "\n";
		return nullptr;
	}

	unsigned int ID;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// free image
	stbi_image_free(data);


	std::shared_ptr<Texture> texture = std::make_shared<Texture>(path, ID, width, height);
	return texture;
}