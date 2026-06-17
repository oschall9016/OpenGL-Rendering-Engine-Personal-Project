#pragma once

#include "Entity.h"
#include "SparseSet.h"
#include "c_Renderable.h"
#include "Renderer.h"
#include "ComponentManager.h"
#include "EntityComponentSystem.h"

#include <set>

class s_Render
{
public:
	s_Render(Renderer& renderer, EntityComponentSystem& ecs);
	void RenderEntitites();

	EntityComponentSystem& ecs;
	Renderer& renderer;
};