#pragma once

#include "Shader.h"
//#include "Mesh.h"
#include "NewMesh.h"

#include <vector>
#include <string>

class Model
{
public:
	Model(std::vector<NewMesh> meshes);
	void Draw(Shader& shader);
private:
	std::vector<NewMesh> meshes;
};