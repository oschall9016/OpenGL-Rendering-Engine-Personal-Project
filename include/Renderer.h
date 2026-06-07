#pragma once

#include "Model.h"
#include "Shader.h"

class Renderer
{
public:
	Renderer();
	void RenderModel(Model& model, Shader& shader);
	void RenderMesh(Mesh& mesh, Shader& shader);
private:
	
	
};
