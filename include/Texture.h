#pragma once

#include <string>

class Texture
{
	// TODO write getters
public:
	Texture(std::string path,unsigned int ID, int width, int height);
	~Texture();

	unsigned int ID;
	int width, height;
	std::string path;



};