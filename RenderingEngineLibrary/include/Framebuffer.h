#pragma once

#include "Shader.h"
#include <memory>

class Framebuffer
{
public:
	Framebuffer(int width, int height);
	~Framebuffer();

	void Bind();
	void Unbind(int screenWidth, int screenHeight);

	const unsigned int& GetFramebufferID();
	const unsigned int& GetTextureColorbufferID();
	const unsigned int& GetQuadVAO();

private:

	void quickQuad();

	unsigned int framebufferID;
	unsigned int textureColorbufferID;

	unsigned int quadVAO, quadVBO, RBO;

	int width;
	int height;
};