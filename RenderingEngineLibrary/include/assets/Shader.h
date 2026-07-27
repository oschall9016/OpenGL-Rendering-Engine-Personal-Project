#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader
{
public:

	Shader(const std::string& vertShaderPath, const std::string& fragShaderPath);
	~Shader();

	void use();
	unsigned int getID() const;

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	

private:

	std::string FilePathToString(const std::string& fileSource);
	unsigned int ID;
};