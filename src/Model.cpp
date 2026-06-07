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