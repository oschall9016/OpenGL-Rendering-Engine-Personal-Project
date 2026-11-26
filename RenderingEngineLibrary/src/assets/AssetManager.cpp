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

std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string& path, bool needsFlipped)
{
	// if texture is in the list
	if (Textures.find(path) != Textures.end())
	{
		return Textures.at(path);
	}

	// texture not in list, needs imported
	std::shared_ptr<Texture> importedTexture = TextureImporter::ImportTexture(path, needsFlipped);
	if (importedTexture == nullptr)
	{
		std::cout << "Texture: " << path << " Failed To Load \n";
		return nullptr;
	}

	Textures.insert({ path, importedTexture });
	return Textures.at(path);
}

std::shared_ptr<Texture> AssetManager::LoadCubeMapTexture(const std::string& path, std::vector<std::string>& cubeFaces, bool needsFlipped)
{
	// if texture is in the list
	if (Textures.find(path) != Textures.end())
	{
		return Textures.at(path);
	}

	// texture not in list, needs imported
	std::shared_ptr<Texture> importedTexture = TextureImporter::ImportCubemapTexture(path, cubeFaces, needsFlipped);
	if (importedTexture == nullptr)
	{
		std::cout << "Cubemap Texture: " << path << " Failed To Load \n";
		return nullptr;
	}

	Textures.insert({ path, importedTexture });
	return Textures.at(path);
}

std::shared_ptr<Model> AssetManager::LoadModel(const std::string& path, bool flipTextures)
{
	// if model is in the list
	if (Models.find(path) != Models.end())
	{
		return Models.at(path);
	}

	// model not in list, needs imported
	ModelImporter mImporter(*this);
	std::shared_ptr<Model> importedModel = mImporter.ImportModel(path, flipTextures);
	if (importedModel == nullptr)
	{
		std::cout << "Model: " << path << " Failed To Load \n";
		return nullptr;
	}

	Models.insert({ path, importedModel });
	return Models.at(path); 
}

std::shared_ptr<Model> AssetManager::LoadModel(const std::string& name, std::shared_ptr<Model> model)
{
	// if model is in the list
	if (Models.find(name) != Models.end())
	{
		return Models.at(name);
	}

	Models.insert({ name, model });
	return Models.at(name);
}

std::shared_ptr<Shader> AssetManager::LoadShader(const std::string& name, const std::string& vPath, const std::string& fPath)
{
	// if shader is in the list
	if (Shaders.find(name) != Shaders.end())
	{
		return Shaders.at(name);
	}

	// if shader not in list, needs imported
	std::shared_ptr<Shader> shaderPointer = std::make_shared<Shader>(vPath, fPath);
	Shaders.insert({name, shaderPointer});
	return Shaders.at(name);
}

std::shared_ptr<Shader> AssetManager::GetShader(const std::string& name)
{
	// if shader is in the list
	if (Shaders.find(name) != Shaders.end())
	{
		return Shaders.at(name);
	}
	std::cout << "Shader:" << name << " Not Found\n";
	return nullptr;
}