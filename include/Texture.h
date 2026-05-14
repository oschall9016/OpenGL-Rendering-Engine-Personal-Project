#pragma once

#include <string>

class Texture
{
public:
	Texture(std::string path,unsigned int ID, int width, int height);
	~Texture();
private:
	unsigned int ID;
	int width, height;
	std::string path;
};