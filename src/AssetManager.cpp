#pragma once

#include "AssetManager.h"

#include "Texture.h"
#include "Model.h"

#include "ModelImporter.h"
#include "TextureImporter.h"

#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>

std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string& path)
{
	// if texture is in the list
	if (Textures.find(path) != Textures.end())
	{
		std::cout << "Texture: " << path << " Is Already Loaded \n";
		return Textures.at(path);
	}

	// texture not in list, needs imported
	std::shared_ptr<Texture> importedTexture = TextureImporter::ImportTexture(path, *this);
	if (importedTexture == nullptr)
	{
		std::cout << "Texture: " << path << " Failed To Load \n";
		return nullptr;
	}

	Textures.insert({ path, importedTexture });
	return Textures.at(path);
}

std::shared_ptr<Model> AssetManager::LoadModel(const std::string& path)
{
	// if model is in the list
	if (Models.find(path) != Models.end())
	{
		std::cout << "Model: " << path << " Is Already Loaded \n";
		return Models.at(path);
	}

	// model not in list, needs imported
	ModelImporter mImporter(*this);
	std::shared_ptr<Model> importedModel = mImporter.ImportModel(path);
	if (importedModel == nullptr)
	{
		std::cout << "Model: " << path << " Failed To Load \n";
		return nullptr;
	}

	Models.insert({ path, importedModel });
	return Models.at(path); 
}

std::shared_ptr<Shader> AssetManager::LoadShader(const std::string& name, const std::string& vPath, const std::string& fPath)
{
	// if shader is in the list
	if (Shaders.find(name) != Shaders.end())
	{
		std::cout << "Shader: " << name << " Is Already Loaded \n";
		return Shaders.at(name);
	}

	// if shader not in list, needs imported
	std::shared_ptr<Shader> shaderPointer = std::make_shared<Shader>(vPath, fPath);
	Shaders.insert({name, shaderPointer});
	return Shaders.at(name);
}




