#include "Shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

// TODO: - clean
//       - seperate shader creation into seperate functions?
Shader::Shader(const std::string& vertShaderPath, const std::string& fragShaderPath)
{
	std::string vertShaderString = FilePathToString(vertShaderPath);
	std::string fragShaderString = FilePathToString(fragShaderPath);

    const char* vertShaderSource = vertShaderString.c_str();
    const char* fragShaderSource = fragShaderString.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(ID);
}

unsigned int Shader::getID() const
{
    return ID;
}

std::string Shader::FilePathToString(const std::string& fileSource)
{
	std::ifstream file(fileSource);
	if (!file.is_open())
	{
		std::cout << "Error Opening " << fileSource << "\n";
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}