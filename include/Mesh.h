#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

/*
struct Texture
{
	std::string path;
	std::string type;
	unsigned int id;
};
*/

class Mesh
{
public:
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	//std::vector<Texture>      textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices/*, std::vector<Texture> textures*/);
	void Draw(Shader &shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};