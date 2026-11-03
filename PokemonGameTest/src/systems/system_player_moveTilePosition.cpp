#include "system_player_moveTilePosition.h"

#include "EntityComponentSystem.h"

#include "component_player_currentState.h"
#include "component_tilemapPosition.h"

#include "Tile.h"
#include "Warp.h"

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

		float cooldownTime = currentState->moveSpeed;
	
		auto& tileX = tilemapPosition->currentX;
		auto& tileZ = tilemapPosition->currentZ;
		auto& tileY = tilemapPosition->currentY;

		float stepSize = 1.0f;

		// we are in a warp tile, step off it
		if (currentState->warping)
		{
			ExitWarp(currentState, tilemapPosition, stepSize);
			continue;
		}

		// we stepped on a warp tile, warp to new position
		if (tilemapPosition->currentTileMap->GetTile(tileX, tileZ).signature & Tile_Type::WARP)
		{
			EnterWarp(currentState, tilemapPosition);
			continue;

		}

		// if the sprite is facing a different direction, change direction instead of moving
		if (inputDirection != spriteDirection && currentState->keyState == Key_State::RELEASED)
		{
			spriteDirection = inputDirection;
			currentState->moveRequest = false;
			continue;
		}

		// only allow movement when time equal to the movespeed has passed
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
		
		// check collision and move horizontally if able
		if (inputDirection == Input_Direction::UP && CheckCollision(tilemapPosition, tileX, tileZ - stepSize))
		{
			tileZ -= stepSize;
		}

		else if (inputDirection == Input_Direction::DOWN && CheckCollision(tilemapPosition, tileX, tileZ + stepSize))
		{
			tileZ += stepSize;
		}

		else if (inputDirection == Input_Direction::LEFT && CheckCollision(tilemapPosition, tileX - stepSize, tileZ))
		{
			tileX -= stepSize;
		}

		else if (inputDirection == Input_Direction::RIGHT && CheckCollision(tilemapPosition, tileX + stepSize, tileZ))
		{
			tileX += stepSize;
		}
	
		// move vertically
		tileY = tilemapPosition->currentTileMap->GetTileHeight(tileX, tileZ);
		
		
		currentState->spriteDirection = inputDirection;

		// request has been processed
		currentState->moveRequest = false;
		
		// debug
		//std::cout << "\n";
		//std::cout << "[ " << tilemapPosition->currentX << " , " << tilemapPosition->currentZ << " ]";
		//std::cout << "\n";
	}
}

void system_player_moveTilePosition::EnterWarp(component_player_currentState* currentState, component_tilemapPosition* tilemapPosition)
{
	currentState->warping = true;
	currentState->inputLocked = true;

	auto& tileX = tilemapPosition->currentX;
	auto& tileZ = tilemapPosition->currentZ;

	// get tile's warp index
	auto& warpIndex = tilemapPosition->currentTileMap->GetTile(tileX, tileZ).warpIndex;

	// get warp struct from list
	auto& warp = tilemapPosition->currentTileMap->mapWarps[warpIndex];

	// swap current position to other side of warp
	warp->currentIndex = 1 - warp->currentIndex;

	// update currentMap
	tilemapPosition->gameWorld->currentMap = warp->destinations[warp->currentIndex].gameMap;

	// update tilemap
	tilemapPosition->currentTileMap = &tilemapPosition->gameWorld->currentMap->GetTilemap();

	// update character position
	tileX = warp->destinations[warp->currentIndex].x;
	tileZ = warp->destinations[warp->currentIndex].z;
}

void system_player_moveTilePosition::ExitWarp(component_player_currentState* currentState, component_tilemapPosition* tilemapPosition, float stepSize)
{
	auto& tileX = tilemapPosition->currentX;
	auto& tileZ = tilemapPosition->currentZ;

	auto& spriteDirection = currentState->spriteDirection;
	auto& inputDirection = currentState->currentDirection;

	// get tile's warp index
	auto& warpIndex = tilemapPosition->currentTileMap->GetTile(tileX, tileZ).warpIndex;

	// get warp struct from list
	auto& warp = tilemapPosition->currentTileMap->mapWarps[warpIndex];

	// move one step in the exit direction
	// NOTE: currently bypasses collision detection
	switch (warp->destinations[warp->currentIndex].exitDirection)
	{
	case Exit_Direction::UP:
		spriteDirection = Input_Direction::UP;
		inputDirection = Input_Direction::UP;
		tileZ -= stepSize;
		break;
	case Exit_Direction::DOWN:
		spriteDirection = Input_Direction::DOWN;
		inputDirection = Input_Direction::DOWN;
		tileZ += stepSize;
		break;
	case Exit_Direction::LEFT:
		spriteDirection = Input_Direction::LEFT;
		inputDirection = Input_Direction::LEFT;
		tileX -= stepSize;
		break;
	case Exit_Direction::RIGHT:
		spriteDirection = Input_Direction::RIGHT;
		inputDirection = Input_Direction::RIGHT;
		tileX += stepSize;
		break;
	}

	currentState->inputLocked = false;
	currentState->warping = false;
}

bool system_player_moveTilePosition::CheckCollision(component_tilemapPosition* tilemapPosition, float x, float z)
{

	float mapXSize = (float)tilemapPosition->currentTileMap->mapXSize;
	float mapZSize = (float)tilemapPosition->currentTileMap->mapZSize;

	if (x < 0 || z < 0 || x >= mapXSize || z >= mapZSize)
	{
		return false;
	}

	Tile currentTile = tilemapPosition->currentTileMap->GetTile(tilemapPosition->currentX, tilemapPosition->currentZ);
	Tile nextTile = tilemapPosition->currentTileMap->GetTile(x, z);

	float currTileHeight = tilemapPosition->currentTileMap->GetTileHeight(tilemapPosition->currentX, tilemapPosition->currentZ);
	float nextTileHeight = tilemapPosition->currentTileMap->GetTileHeight(x, z);

	auto& tileY = tilemapPosition->currentY;

	bool heightDiffIs1 = std::abs(nextTileHeight - currTileHeight) == 1.0f;
	bool heightDiffUnderPoint5 = std::abs(nextTileHeight - currTileHeight) <= 0.5f;
	bool bothSlopes = nextTile.signature & Tile_Type::SLOPE && currentTile.signature & Tile_Type::SLOPE;

	if (nextTile.signature & Tile_Type::COLLIDER)
	{
		return false;
	}
	else if (bothSlopes)
	{
		return heightDiffIs1;
	}
	else
	{
		return heightDiffUnderPoint5;
	}
}