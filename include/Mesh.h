#pragma once

#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>

#include <vector>
#include <memory>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures);

	const std::vector<Vertex>& GetVertices();
	const std::vector<unsigned int>& getIndices();
	const std::vector<std::shared_ptr<Texture>>& getTextures();
	const unsigned int& GetVAO();
private:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<std::shared_ptr<Texture>> textures;

	unsigned int VAO, VBO, EBO;

	void setUpMesh();
};
