#pragma once

#include "Model.h"
#include "NewMesh.h"
#include "Texture.h"

#include <assimp/scene.h>

class NewModelLoader
{
public:
	Model LoadModel(const std::string& path);
	Texture LoadTexture(const std::string& path);
private:
	void TraverseAssimpScene(const aiScene* scene, std::vector<NewMesh>& meshes);
	NewMesh CreateMesh(aiMesh* mesh);
};