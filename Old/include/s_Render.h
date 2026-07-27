#pragma once

#include "System.h"
#include "Entity.h"
#include "SparseSet.h"
#include "c_Renderable.h"
#include "Renderer.h"
#include "ComponentManager.h"
#include "EntityComponentSystem.h"
#include "Camera.h"

#include <set>

class s_Render : public System
{
public:
	s_Render(Renderer& renderer, EntityComponentSystem& ecs, Camera& camera);
	void RenderEntitites();

	EntityComponentSystem& ecs;
	Renderer& renderer;
	Camera& camera;
};