#pragma once

#include <string>

class Texture
{
public:
	Texture(std::string path,unsigned int ID, int width, int height);
	~Texture();

	const unsigned int& GetID();
	const int& GetWidth();
	const int& GetHeight();
	const std::string& GetPath();
	
private:
	unsigned int ID;
	int width, height;
	std::string path;
};