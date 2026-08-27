#pragma once

#include "System.h"

#include "EntityComponentSystem.h"
#include "SDLInput.h"

#include "component_player_currentState.h"

/*
* Components:
*	- component_player_currentState
*/

class system_player_inputDetection : public System
{
public:
	system_player_inputDetection(EntityComponentSystem& ecs, SDLInput& input);
	void Update(float dt);
private:
	void updateComponent(component_player_currentState* state, Input_Direction direction);
	
	EntityComponentSystem& ecs;
	SDLInput& input;

	float holdTimer = 0.0f;
};