#pragma once

#include <string>

class Shader
{
public:

	Shader(const std::string& vertShaderPath, const std::string& fragShaderPath);
	void use();
	unsigned int getID() const;

private:

	std::string FilePathToString(const std::string& fileSource);
	unsigned int ID;
};