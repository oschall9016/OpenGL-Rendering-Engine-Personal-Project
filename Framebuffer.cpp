#include "Framebuffer.h"
#include "Shader.h"

#include <glad/glad.h>

#include <iostream>
#include <memory>

Framebuffer::Framebuffer(int width, int height)
{
	this->width = width;
	this->height = height;

	// create fullscreen quad
	quadVAO = quickQuad();

	// generate framebuffer
	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

	// generate textureColorBuffer
	glGenTextures(1, &textureColorbufferID);
	glBindTexture(GL_TEXTURE_2D, textureColorbufferID);

	// create texture?
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbufferID, 0);

	// create renderbuffer object for depth
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // what is GL_DEPTH24_STENCIl8?
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// TODO: delete VAO and VBO?
Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &framebufferID);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glViewport(0, 0, width, height);
}

void Framebuffer::Unbind(int screenWidth, int screenHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screenWidth, screenHeight);
}

unsigned int Framebuffer::quickQuad()
{
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	return quadVAO;

}

const unsigned int& Framebuffer::GetFramebufferID()
{
	return framebufferID;
}

const unsigned int& Framebuffer::GetTextureColorbufferID()
{
	return textureColorbufferID;
}

const unsigned int& Framebuffer::GetQuadVAO()
{
	return quadVAO;
}