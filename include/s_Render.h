#pragma once

#include "Entity.h"
#include "SparseSet.h"
#include "c_Renderable.h"
#include "Renderer.h"
#include "TestScene.h"
#include "ComponentManager.h"

#include <set>

class s_Render
{
public:
	s_Render(Renderer& renderer, ComponentManager& manager);
	void RenderEntitites();

	ComponentManager& manager;
	std::set<Entity> entities;
	Renderer& renderer;
};