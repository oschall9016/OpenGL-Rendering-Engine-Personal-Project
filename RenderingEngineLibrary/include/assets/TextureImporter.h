#pragma once

#include "Texture.h"

#include "AssetManager.h"

#include <memory>
#include <string>
#include <vector>

#include <assimp/scene.h>

class TextureImporter
{
public:
	static std::shared_ptr<Texture> ImportTexture(const std::string& path, AssetManager& manager, bool needsFlipped);
	static std::shared_ptr<Texture> ImportCubemapTexture(const std::string& path, std::vector<std::string>& cubeFaces, AssetManager& manager, bool needsFlipped);
};