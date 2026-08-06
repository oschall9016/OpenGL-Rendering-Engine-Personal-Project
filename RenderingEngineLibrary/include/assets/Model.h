#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <memory>

class Model
{
public:
	Model(std::vector<std::shared_ptr<Mesh>> meshes);

	std::vector<std::shared_ptr<Mesh>>& GetMeshes();

	static std::shared_ptr<Model> CreateQuad(std::shared_ptr<Texture> texture);
	static std::shared_ptr<Model> CreateEmptyQuad();
private:
	std::vector<std::shared_ptr<Mesh>> meshes;
};