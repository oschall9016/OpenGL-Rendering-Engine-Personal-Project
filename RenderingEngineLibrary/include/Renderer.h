#pragma once

#include "Model.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Skybox.h"
#include "Camera.h"

#include <memory>

class Renderer
{
public:
	Renderer();

	void RenderModel(Model& model, Shader& shader);
	void RenderMesh(Mesh& mesh, Shader& shader);

	void Clear();

	void RenderFramebufferQuad(Framebuffer& framebuffer, Shader& shader);
	void RenderSkybox(Skybox& skybox, Shader& shader, Camera& camera);



private:
	
	
};
