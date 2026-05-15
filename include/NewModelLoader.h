#pragma once

#include "Model.h"
#include "NewMesh.h"
#include "Texture.h"

#include <memory>

#include <assimp/scene.h>


// maybe make this a namespace?
class NewModelLoader
{
public:

	static std::shared_ptr<Model> LoadModel(const std::string& path);
	static std::shared_ptr<Texture> LoadTexture(const std::string& path);
private:
	static void TraverseAssimpScene(const aiScene* scene, std::vector<NewMesh>& meshes);
	static NewMesh CreateMesh(aiMesh* mesh);
};