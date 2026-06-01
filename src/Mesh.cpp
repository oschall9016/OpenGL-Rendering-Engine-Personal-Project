#include "Mesh.h"
#include "Shader.h"

#include "glad/glad.h"
#include <glm/glm.hpp>

#include <cstddef>
#include <vector>

Mesh::Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setUpMesh();
}

void Mesh::Draw(Shader& shader)
{

	unsigned int diffuseNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = "texture_diffuse";
		number = std::to_string(diffuseNr++);

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.getID(), (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i]->ID);

	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setUpMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// texCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	glBindVertexArray(0);
}

const std::vector<Vertex>& Mesh::GetVertices()
{
	return vertices;
}

const std::vector<unsigned int>& Mesh::getIndices()
{
	return indices;
}

const std::vector<std::shared_ptr<Texture>>& Mesh::getTextures()
{
	return textures;
}

