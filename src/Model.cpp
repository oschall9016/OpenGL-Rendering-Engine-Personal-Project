#include "Model.h"
#include "Shader.h"

#include <vector>
#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>

#include <stb_image.h>

Model::Model(std::vector<Mesh> meshes)
{
	this->meshes = meshes;
}

std::vector<Mesh>& Model::GetMeshes()
{
	return meshes;
}

Model Model::CreateQuad(std::shared_ptr<Texture> texture)
{
	glm::vec3 positions[] = {
		{  0.5f,  0.5f, 0.0f },  // top right
		{  0.5f, -0.5f, 0.0f },  // bottom right
		{ -0.5f, -0.5f, 0.0f },  // bottom left
		{ -0.5f,  0.5f, 0.0f }   // top left 
	};

	glm::vec2 texCoords[] =
	{
		{1.0f, 1.0f}, // top right
		{1.0f, 0.0f}, // bottom right
		{0.0f, 0.0f}, // bottom left
		{0.0f, 1.0f}  // top left
	};

	std::vector<Vertex> vertices = // no normals
	{
		{positions[0],glm::vec3(0.0f),texCoords[0]},
		{positions[1],glm::vec3(0.0f),texCoords[1]},
		{positions[2],glm::vec3(0.0f),texCoords[2]},
		{positions[3],glm::vec3(0.0f),texCoords[3]}
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	std::vector<std::shared_ptr<Texture>> textures = { texture };

	Mesh quadMesh(vertices, indices, textures);
	
	std::vector<Mesh> quadMeshVector = { quadMesh };

	return Model(quadMeshVector);
}