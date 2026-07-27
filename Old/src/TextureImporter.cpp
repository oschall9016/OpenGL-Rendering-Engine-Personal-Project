#include "TextureImporter.h"

#include "Texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <stb_image.h>

#include <string>
#include <iostream>
#include <memory>

std::shared_ptr<Texture> TextureImporter::ImportTexture(const std::string& path, AssetManager& manager, bool needsFlipped)
{
	stbi_set_flip_vertically_on_load(needsFlipped);

	int width, height, nrChannels;
	unsigned char* data;
	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cout << "Failed to Import Texture: " << path << "\n";
		stbi_image_free(data);
		return nullptr;
	}

	GLenum format;
	if (nrChannels == 1) format = GL_RED;
	else if (nrChannels == 3) format = GL_RGB;
	else if (nrChannels == 4) format = GL_RGBA;
	else
	{
		std::cout << "Unrecognized Channel Count" << "\n";
		stbi_image_free(data);
		return nullptr;
	}

	unsigned int ID;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // changed to nearest for pixelated look
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// free image
	stbi_image_free(data);

	std::shared_ptr<Texture> texture = std::make_shared<Texture>(path, ID, width, height);
	return texture;
}

std::shared_ptr<Texture> TextureImporter::ImportCubemapTexture(const std::string& path, std::vector<std::string>& cubeFaces, AssetManager& manager, bool needsFlipped)
{
	stbi_set_flip_vertically_on_load(needsFlipped);

	unsigned int ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	int width, height, nrChannels;
	unsigned char* data;
	for (int i = 0; i < cubeFaces.size(); i++)
	{
		data = stbi_load(cubeFaces[i].c_str(), &width, &height, &nrChannels, 0);
		if (!data)
		{
			std::cout << "Failed to Import Cubemap Texture: " << cubeFaces[i] << "\n";
			stbi_image_free(data);
			return nullptr;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	std::shared_ptr<Texture> texture = std::make_shared<Texture>(path, ID, width, height);
	return texture;
}