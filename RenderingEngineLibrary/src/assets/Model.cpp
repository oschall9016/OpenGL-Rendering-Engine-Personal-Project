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

Model::Model(std::vector<std::shared_ptr<Mesh>> meshes)
{
	this->meshes = meshes;
}

std::vector<std::shared_ptr<Mesh>>& Model::GetMeshes()
{
	return meshes;
}

std::shared_ptr<Model> Model::CreateQuad(std::shared_ptr<Texture> texture)
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

	std::shared_ptr<Mesh> quadMesh = std::make_shared<Mesh>(vertices, indices, textures);
	
	std::vector<std::shared_ptr<Mesh>> quadMeshVector = { quadMesh };

	return std::make_shared<Model>(quadMeshVector);
}

std::shared_ptr<Model> Model::CreateEmptyQuad() //TODO: dont need a seperate function for this
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

	std::vector<std::shared_ptr<Texture>> textures = { };

	std::shared_ptr<Mesh> quadMesh = std::make_shared<Mesh>(vertices, indices, textures);

	std::vector<std::shared_ptr<Mesh>> quadMeshVector = { quadMesh };

	return std::make_shared<Model>(quadMeshVector);
}

std::shared_ptr<Model> Model::CreateEmptyCube()
{
    glm::vec3 positions[] =
    {
        // back face
        {-0.5f, -0.5f, -0.5f}, // 0 bottom left
        { 0.5f, -0.5f, -0.5f}, // 1 bottom right
        { 0.5f,  0.5f, -0.5f}, // 2 top right
        {-0.5f,  0.5f, -0.5f}, // 3 top left

        // front face
        {-0.5f, -0.5f,  0.5f}, // 4 bottom left
        { 0.5f, -0.5f,  0.5f}, // 5 bottom right
        { 0.5f,  0.5f,  0.5f}, // 6 top right
        {-0.5f,  0.5f,  0.5f}, // 7 top left

        // left face
        {-0.5f, -0.5f, -0.5f}, // 8 bottom back
        {-0.5f, -0.5f,  0.5f}, // 9 bottom front
        {-0.5f,  0.5f,  0.5f}, // 10 top front
        {-0.5f,  0.5f, -0.5f}, // 11 top back

        // right face
        { 0.5f, -0.5f,  0.5f}, // 12 bottom front
        { 0.5f, -0.5f, -0.5f}, // 13 bottom back
        { 0.5f,  0.5f, -0.5f}, // 14 top back
        { 0.5f,  0.5f,  0.5f}, // 15 top front

        // bottom face
        {-0.5f, -0.5f, -0.5f}, // 16 back left
        { 0.5f, -0.5f, -0.5f}, // 17 back right
        { 0.5f, -0.5f,  0.5f}, // 18 front right
        {-0.5f, -0.5f,  0.5f}, // 19 front left

        // top face
        {-0.5f,  0.5f, -0.5f}, // 20 back left
        { 0.5f,  0.5f, -0.5f}, // 21 back right
        { 0.5f,  0.5f,  0.5f}, // 22 front right
        {-0.5f,  0.5f,  0.5f}  // 23 front left
    };

    glm::vec2 texCoords[] =
    {
        // back
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},

        // front
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},

        // left
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},

        // right
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},

        // bottom
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},

        // top
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    std::vector<Vertex> vertices =
    {
        // back
        {positions[0],  glm::vec3(0.0f), texCoords[0]},
        {positions[1],  glm::vec3(0.0f), texCoords[1]},
        {positions[2],  glm::vec3(0.0f), texCoords[2]},
        {positions[3],  glm::vec3(0.0f), texCoords[3]},

        // front
        {positions[4],  glm::vec3(0.0f), texCoords[4]},
        {positions[5],  glm::vec3(0.0f), texCoords[5]},
        {positions[6],  glm::vec3(0.0f), texCoords[6]},
        {positions[7],  glm::vec3(0.0f), texCoords[7]},

        // left
        {positions[8],  glm::vec3(0.0f), texCoords[8]},
        {positions[9],  glm::vec3(0.0f), texCoords[9]},
        {positions[10], glm::vec3(0.0f), texCoords[10]},
        {positions[11], glm::vec3(0.0f), texCoords[11]},

        // right
        {positions[12], glm::vec3(0.0f), texCoords[12]},
        {positions[13], glm::vec3(0.0f), texCoords[13]},
        {positions[14], glm::vec3(0.0f), texCoords[14]},
        {positions[15], glm::vec3(0.0f), texCoords[15]},

        // bottom
        {positions[16], glm::vec3(0.0f), texCoords[16]},
        {positions[17], glm::vec3(0.0f), texCoords[17]},
        {positions[18], glm::vec3(0.0f), texCoords[18]},
        {positions[19], glm::vec3(0.0f), texCoords[19]},

        // top
        {positions[20], glm::vec3(0.0f), texCoords[20]},
        {positions[21], glm::vec3(0.0f), texCoords[21]},
        {positions[22], glm::vec3(0.0f), texCoords[22]},
        {positions[23], glm::vec3(0.0f), texCoords[23]}
    };

    std::vector<unsigned int> indices =
    {
        // back
        0, 1, 2,
        2, 3, 0,

        // front
        4, 5, 6,
        6, 7, 4,

        // left
        8, 9, 10,
        10, 11, 8,

        // right
        12, 13, 14,
        14, 15, 12,

        // bottom
        16, 17, 18,
        18, 19, 16,

        // top
        20, 21, 22,
        22, 23, 20
    };

    std::vector<std::shared_ptr<Texture>> textures = { };

    std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>(vertices, indices, textures);

    std::vector<std::shared_ptr<Mesh>> cubeMeshVector = { cubeMesh };

    return std::make_shared<Model>(cubeMeshVector);
}