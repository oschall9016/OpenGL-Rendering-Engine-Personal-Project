#include "AssetImporter.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "AssetManager.h"

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

std::shared_ptr<Model> AssetImporter::LoadModel(const std::string& path, AssetManager& manager)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); // look into the flags

	if (scene == nullptr)
	{
		std::cout << "Failed to Load Model: " << importer.GetErrorString() << "\n";
	}

	std::vector<Mesh> meshes;

	TraverseAssimpScene(path, scene, meshes, manager);

	std::shared_ptr<Model> model = make_shared<Model>(meshes);

	return model;

}

void AssetImporter::TraverseAssimpScene(const std::string& directory, const aiScene* scene, std::vector<Mesh>& meshes, AssetManager& manager)
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
			Mesh tempMesh = CreateMesh(directory,scene,scene->mMeshes[meshIndex],manager);
			meshes.push_back(tempMesh);
		}

		for (unsigned int i = 0; i < curNode->mNumChildren; i++)
		{
			nodeStack.push(curNode->mChildren[i]);
		}

	}
}

Mesh AssetImporter::CreateMesh(const std::string& directory, const aiScene* scene, aiMesh* mesh, AssetManager& manager)
{
	std::vector<Vertex> tempVertices;
	std::vector<unsigned int> tempIndices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex tempVertex{};

		// store positions
		tempVertex.position = glm::vec3
		(
			mesh->mVertices[i].x, 
			mesh->mVertices[i].y, 
			mesh->mVertices[i].z
		);

		// store normals
		if (mesh->HasNormals())
		{
			tempVertex.normal = glm::vec3
			(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);
		}

		// store texCoords (for main texture)
		if (mesh->HasTextureCoords(0))
		{
			tempVertex.texCoord = glm::vec2
			(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}

		tempVertices.push_back(tempVertex);
	}

	// store indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			tempIndices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<std::shared_ptr<Texture>> tempTextures;
	
	// diffuse
	std::vector<std::shared_ptr<Texture>> diffuseTextures = importTextures(directory, mat, aiTextureType_DIFFUSE, manager);
	tempTextures.insert(tempTextures.end(), diffuseTextures.begin(), diffuseTextures.end());

	return Mesh(tempVertices, tempIndices, tempTextures);
}

std::vector<std::shared_ptr<Texture>> AssetImporter::importTextures(const std::string& directory,aiMaterial* mat, aiTextureType type, AssetManager &manager)
{
	std::vector<std::shared_ptr<Texture>> tempTextures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString aiPath; 
		mat->GetTexture(type, i, &aiPath);
		std::string path = aiPath.C_Str();

		std::string absPath = directory.substr(0, directory.find_last_of('/'));
		absPath += "/" + path;

		tempTextures.push_back(manager.LoadTexture(absPath));
	}
	return tempTextures;
}

std::shared_ptr<Texture> AssetImporter::LoadTexture(const std::string& path)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* data;
	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cout << "Failed to Import Texture: " << path << "\n";
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