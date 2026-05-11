#include "NewMesh.h"
#include "Shader.h"

#include "glad/glad.h"
#include <glm/glm.hpp>

#include <vector>



NewMesh::NewMesh(std::vector<NewVertex> vertices,std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	setUpMesh();
}

void NewMesh::Draw(Shader& shader)
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void NewMesh::setUpMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(NewVertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NewVertex), (void*)0);

	glBindVertexArray(0);
}