#pragma once

#include "System.h"

#include "EntityComponentSystem.h"
#include "Renderer.h"
#include "Camera.h"

// components:
//  model
//  transform

class system_RenderEntities : public System
{
public:
	system_RenderEntities(Renderer& renderer, EntityComponentSystem& ecs, Camera& camera);

	void RenderEntities();
	
	EntityComponentSystem& ecs;
	Renderer& renderer;
	Camera& camera;
};