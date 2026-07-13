#pragma once

#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

#include <vector>
#include <string>
#include <memory>

class Skybox
{
public:
	Skybox(std::shared_ptr<Texture> texture);
	~Skybox();

	const unsigned int& GetCubeVAO();
	const unsigned int& GetTextureID();

private:

	unsigned int QuickCube();

	std::shared_ptr<Texture> texture;

	unsigned int cubeVAO;
};