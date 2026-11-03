#pragma once

#include "Tilemap.h"
#include "GameWorld.h"

struct component_tilemapPosition
{
	float currentX = 0.0f;
	float currentZ = 0.0f;

	Tilemap* currentTileMap = nullptr;
	GameWorld* gameWorld = nullptr;

	float currentY = 0.0f;
};