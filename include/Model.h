#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

class Model
{
public:
	Model(std::vector<Mesh> meshes);

	std::vector<Mesh>& GetMeshes();

	static Model CreateQuad(std::shared_ptr<Texture> texture);
private:
	std::vector<Mesh> meshes;
};