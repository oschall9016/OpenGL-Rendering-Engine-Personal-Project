#include "NewModelLoader.h"
#include "NewMesh.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <stack>
#include <vector>


Model NewModelLoader::LoadModel(const std::string& path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); // look into the flags

	if (scene == nullptr)
	{
		std::cout << "Failed to Load Model: " << importer.GetErrorString() << "\n";
	}

	std::vector<NewMesh> meshes;

	TraverseAssimpScene(scene, meshes);

	return Model(meshes);

}

void NewModelLoader::TraverseAssimpScene(const aiScene* scene, std::vector<NewMesh>& meshes)
{
	
	std::stack<aiNode*> nodeStack;
	nodeStack.push(scene->mRootNode);

	while (!nodeStack.empty())
	{
		aiNode* curNode = nodeStack.top();
		nodeStack.pop();

		// do stuff with epic node
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