#pragma once

#include "System.h"

#include "EntityComponentSystem.h"

/*
* Components:
*	- component_player_currentState
* 	- component_player_animations
*   - component_Model
*/

class system_player_playAnimation : public System
{
public:
	system_player_playAnimation(EntityComponentSystem& ecs);
	void Update(float dt);
private:
	EntityComponentSystem& ecs;
	float updateCooldown = 0.0f;
};
