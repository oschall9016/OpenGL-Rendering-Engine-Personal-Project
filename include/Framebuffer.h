#pragma once

class Framebuffer
{
public:
	Framebuffer(int width, int height);

	void Bind();
	void Unbind();

	const unsigned int& GetFrameBufferID();
	const unsigned int& GetTextureColorBufferID();

private:
	unsigned int frameBufferID;
	unsigned int textureColorBufferID;
	int width;
	int height;
};