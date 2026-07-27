#pragma once

#include "Model.h"
#include "Shader.h"

#include <memory>

struct c_Renderable
{
	std::shared_ptr<Model> model;
	std::shared_ptr<Shader> shader;
};