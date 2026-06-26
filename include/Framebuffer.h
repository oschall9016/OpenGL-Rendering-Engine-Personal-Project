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

	void renderFramebufferQuad(std::shared_ptr<Shader> shader);
	unsigned int quickQuad();

	const unsigned int& GetFramebufferID();
	const unsigned int& GetTextureColorbufferID();

private:
	unsigned int framebufferID;
	unsigned int textureColorbufferID;

	unsigned int quadVAO;

	int width;
	int height;
};