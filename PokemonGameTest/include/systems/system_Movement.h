#pragma once

#include "System.h"

#include "component_Movement.h"

#include "SDLInput.h"
#include "Shader.h"
#include "EntityComponentSystem.h"

// components:
//	movement
//  model

class system_Movement : public System
{
public:
	system_Movement(EntityComponentSystem& ecs, SDLInput& input);
	void Update();
private:

	void UpdateMovementAnim(component_Movement* movement, Direction movDirection);

	EntityComponentSystem& ecs;
	SDLInput& input;
};