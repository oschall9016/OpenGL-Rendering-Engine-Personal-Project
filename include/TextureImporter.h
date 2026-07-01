#pragma once

#include "Texture.h"

#include "AssetManager.h"

#include <memory>

#include <assimp/scene.h>

class TextureImporter
{
public:
	static std::shared_ptr<Texture> ImportTexture(const std::string& path, AssetManager& manager, bool needsFlipped);
};