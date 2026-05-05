#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model
{
public:
	Model(std::string path);
	void Draw(Shader& shader);
private:
	std::vector<Mesh> meshes;
	std::string directory;

	std::vector<Texture> textures_loaded;

	unsigned int TextureFromFile(const char* path, const std::string& directory);

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};