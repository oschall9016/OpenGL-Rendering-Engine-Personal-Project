#include "Texture.h"

#include <glad/glad.h>

#include <string>

Texture::Texture(std::string path, unsigned int ID, int width, int height)
{
	this->path = path;
	this->ID = ID;
	this->width = width;
	this->height = height;
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}