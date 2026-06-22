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


const unsigned int& Texture::GetID()
{
	return ID;
}

const int& Texture::GetWidth()
{
	return width;
}

const int& Texture::GetHeight()
{
	return height;
}

const std::string& Texture::GetPath()
{
	return path;
}