#pragma once

#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "AssetManager.h"

#include <memory>

#include <assimp/scene.h>


// maybe make this a namespace?
class AssetImporter
{
public:

	static std::shared_ptr<Model> LoadModel(const std::string& path, AssetManager& manager);
	static std::shared_ptr<Texture> LoadTexture(const std::string& path);
private:
	static void TraverseAssimpScene(const std::string& directory, const aiScene* scene, std::vector<Mesh>& meshes, AssetManager& manager);
	static Mesh CreateMesh(const std::string& directory, const aiScene* scene, aiMesh* mesh, AssetManager& manager);
	static std::vector<std::shared_ptr<Texture>> importTextures(const std::string& directory, aiMaterial *mat, aiTextureType type, AssetManager& manager);
};