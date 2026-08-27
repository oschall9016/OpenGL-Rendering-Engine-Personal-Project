#pragma once

#include "System.h"
#include "EntityComponentSystem.h"

#include "component_player_currentState.h"
#include "component_tilemapPosition.h"
#include "component_Transform.h"

#include "Camera.h"

/*
* Components:
*	- component_player_currentState
*	- component_tilemapPosition
* 	- component_Transform
*/

class system_player_moveRenderPosition : public System
{
public:
	system_player_moveRenderPosition(EntityComponentSystem& ecs);
	void Update(float dt, Camera& camera);
private:
	EntityComponentSystem& ecs;
};
