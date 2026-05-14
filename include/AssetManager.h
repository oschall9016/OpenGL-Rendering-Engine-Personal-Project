#pragma once

#include "Texture.h"

#include <memory>
#include <map>
#include <string>

class AssetManager
{
public:

private:
	std::map<std::string, std::shared_ptr<Texture>> Textures;
};