#pragma once

#include "Texture.h"
#include "Model.h"
#include "Shader.h"

#include <memory>
#include <unordered_map>
#include <string>

class AssetManager
{
public:
	std::shared_ptr<Texture> LoadTexture(const std::string& path);
	std::shared_ptr<Model> LoadModel(const std::string& path);
	std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& vPath, const std::string& fPath);
private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> Textures;
	std::unordered_map<std::string, std::shared_ptr<Model>> Models;
	std::unordered_map<std::string, std::shared_ptr<Shader>> Shaders;
};