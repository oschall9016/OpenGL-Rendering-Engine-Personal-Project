#pragma once

#include "Model.h"
#include "Mesh.h"
#include "Texture.h"

#include <memory>

#include <assimp/scene.h>


// maybe make this a namespace?
class AssetImporter
{
public:

	static std::shared_ptr<Model> LoadModel(const std::string& path);
	static std::shared_ptr<Texture> LoadTexture(const std::string& path);
private:
	static void TraverseAssimpScene(const aiScene* scene, std::vector<Mesh>& meshes);
	static Mesh CreateMesh(aiMesh* mesh);
};