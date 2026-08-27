#include "system_player_moveTilePosition.h"

#include "EntityComponentSystem.h"

#include "component_player_currentState.h"
#include "component_tilemapPosition.h"

#include <iostream>
#include <cmath>

/*
* Components:
*	- component_player_currentState
*	- component_tilemapPosition
*/

system_player_moveTilePosition::system_player_moveTilePosition(EntityComponentSystem& ecs) : ecs(ecs) {}

void system_player_moveTilePosition::Update(double dt)
{

	for (auto entity : entities)
	{
		auto currentState = ecs.GetComponent<component_player_currentState>(entity);
		auto tilemapPosition = ecs.GetComponent<component_tilemapPosition>(entity);

		auto& inputDirection = currentState->currentDirection;
		auto& spriteDirection = currentState->spriteDirection;

		// only allow movement when time equal to the movespeed has passed
		float cooldownTime = currentState->moveSpeed;

		if (inputDirection != spriteDirection && currentState->currentAction == Input_Action::STANDING && currentState->keyState == Key_State::RELEASED)
		{
			spriteDirection = inputDirection;
			currentState->moveRequest = false;
			continue;
		}

		if (moveCooldown < cooldownTime)
		{
			moveCooldown += dt;
			continue;
		}
		moveCooldown = 0;

		// not moving so skip
		if (!currentState->moveRequest)
		{
			continue;
		}
		
		auto& tileX = tilemapPosition->currentX;
		auto& tileZ = tilemapPosition->currentZ;

		float stepSize = 1.0f;

		if (inputDirection == Input_Direction::UP && CheckCollision(tilemapPosition, tileX, tileZ - stepSize) && (inputDirection == currentState->spriteDirection || currentState->keyState == Key_State::HELD))
		{
			tileZ -= stepSize;
		}

		else if (inputDirection == Input_Direction::DOWN && CheckCollision(tilemapPosition, tileX, tileZ + stepSize) && (inputDirection == currentState->spriteDirection || currentState->keyState == Key_State::HELD))
		{
			tileZ += stepSize;
		}

		else if (inputDirection == Input_Direction::LEFT && CheckCollision(tilemapPosition, tileX - stepSize, tileZ) && (inputDirection == currentState->spriteDirection || currentState->keyState == Key_State::HELD))
		{
			tileX -= stepSize;
		}

		else if (inputDirection == Input_Direction::RIGHT && CheckCollision(tilemapPosition, tileX + stepSize, tileZ) && (inputDirection == currentState->spriteDirection || currentState->keyState == Key_State::HELD))
		{
			tileX += stepSize;
		}
	
		currentState->spriteDirection = inputDirection;

		// request has been processed
		currentState->moveRequest = false;
		
		

		std::cout << "\n";
		std::cout << "[ " << tilemapPosition->currentX << " , " << tilemapPosition->currentZ << " ]";
		std::cout << "\n";
	}
}

bool system_player_moveTilePosition::CheckCollision(component_tilemapPosition* tilemapPosition, float x, float z)
{

	int mapCols = tilemapPosition->currentMap->mapCols;
	int mapRows = tilemapPosition->currentMap->mapRows;

	if (x < 0 || z < 0 || x >= mapCols || z >= mapRows)
	{
		return false;
	}

	Tile tile = tilemapPosition->currentMap->GetTile(z,x);
	
	if (tile.signature == COLLIDER)
	{
		return false;
	}
	else
	{
		return true;
	}
}