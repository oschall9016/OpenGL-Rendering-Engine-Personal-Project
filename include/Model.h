#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model
{
public:
	Model(std::vector<Mesh> meshes);
	void Draw(Shader& shader);
private:
	std::vector<Mesh> meshes;
};