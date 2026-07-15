#pragma once

#include "Model.h"
#include "Mesh.h"

#include "AssetManager.h"

#include <memory>

#include <assimp/scene.h>

class ModelImporter
{
public:
	ModelImporter(AssetManager& manager);
	std::shared_ptr<Model> ImportModel(const std::string& path);
private:
	void TraverseAssimpScene(const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes);
	std::shared_ptr<Mesh> CreateMesh(const aiScene* scene, aiMesh* mesh);
	std::vector<std::shared_ptr<Texture>> importTextures(aiMaterial* mat, aiTextureType type);

	AssetManager manager;
	std::string path;
};