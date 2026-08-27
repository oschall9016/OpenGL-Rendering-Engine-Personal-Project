#pragma once

#include "System.h"

#include "EntityComponentSystem.h"

#include "component_player_currentState.h"
#include "component_tilemapPosition.h"

/*
* Components:
*	- component_player_currentState
*	- component_tilemapPosition
*/

class system_player_moveTilePosition : public System
{
public:
	system_player_moveTilePosition(EntityComponentSystem& ecs);

	void Update(double dt);

private:

	bool CheckCollision(component_tilemapPosition* tilemapPosition, float x, float z);

	EntityComponentSystem& ecs;
	double moveCooldown = 0.0;
};
