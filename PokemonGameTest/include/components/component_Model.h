#pragma once

#include "Model.h"
#include "Shader.h"

#include <memory.h>

struct component_Model
{
	std::shared_ptr<Model> model;
	std::shared_ptr<Shader> shader;
};
