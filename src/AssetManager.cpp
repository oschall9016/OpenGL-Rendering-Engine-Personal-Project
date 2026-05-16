#pragma once

#include "AssetManager.h"

#include "Texture.h"
#include "Model.h"
#include "AssetImporter.h"

#include <memory>
#include <map>
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

	// texture not in list, needs loaded
	Textures.insert({path,AssetImporter::LoadTexture(path)});
	return Textures.at(path);
}

std::shared_ptr<Model> AssetManager::LoadModel(const std::string& path)
{
	// if texture is in the list
	if (Models.find(path) != Models.end())
	{
		std::cout << "Model: " << path << " Is Already Loaded \n";
		return Models.at(path);
	}

	// texture not in list, needs loaded
	Models.insert({ path,AssetImporter::LoadModel(path) });
	return Models.at(path); 
}




