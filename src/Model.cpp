#include "Model.h"
#include "Shader.h"
#include "Mesh.h"

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

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}