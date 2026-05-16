#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

#include <vector>

struct NewVertex
{
	glm::vec3 position;
};

class Mesh
{
public:
	Mesh(std::vector<NewVertex> vertices, std::vector<unsigned int> indices);
	void Draw(Shader& shader);
private:

	std::vector<NewVertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO, VBO, EBO;

	void setUpMesh();
};
