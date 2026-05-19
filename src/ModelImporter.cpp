#include "ModelImporter.h"
#include "Mesh.h"
#include "Model.h"
#include "AssetManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include <memory>

ModelImporter::ModelImporter(AssetManager& manager) : manager(manager){}

std::shared_ptr<Model> ModelImporter::ImportModel(const std::string& path)
{
	this->path = path;
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); //TODO look into other flags

	if (scene == nullptr)
	{
		std::cout << "Failed to Load Model: " << importer.GetErrorString() << "\n";
	}

	std::vector<Mesh> meshes;

	TraverseAssimpScene(scene, meshes);

	std::shared_ptr<Model> model = make_shared<Model>(meshes);

	return model;

}

void ModelImporter::TraverseAssimpScene(const aiScene* scene, std::vector<Mesh>& meshes)
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
			Mesh tempMesh = CreateMesh(scene, scene->mMeshes[meshIndex]);
			meshes.push_back(tempMesh);
		}

		for (unsigned int i = 0; i < curNode->mNumChildren; i++)
		{
			nodeStack.push(curNode->mChildren[i]);
		}

	}
}

Mesh ModelImporter::CreateMesh(const aiScene* scene, aiMesh* mesh)
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
	std::vector<std::shared_ptr<Texture>> diffuseTextures = importTextures(mat, aiTextureType_DIFFUSE);
	tempTextures.insert(tempTextures.end(), diffuseTextures.begin(), diffuseTextures.end());

	return Mesh(tempVertices, tempIndices, tempTextures);
}

std::vector<std::shared_ptr<Texture>> ModelImporter::importTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<std::shared_ptr<Texture>> tempTextures;

	// loop through all textures of given type
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		// get the absolute path to the texture
		aiString aiPath;
		mat->GetTexture(type, i, &aiPath);
		std::string fileName = aiPath.C_Str();

		std::string absPath = path.substr(0, path.find_last_of('/'));
		absPath += "/" + fileName;

		// load texture from manager
		tempTextures.push_back(manager.LoadTexture(absPath));
	}
	return tempTextures;
}