#pragma once

#include "Tilemap.h"
#include "GameWorld.h"

struct component_tilemapPosition
{
	int currentX = 0;
	int currentZ = 0;

	Tilemap* currentTileMap = nullptr;
	GameWorld* gameWorld = nullptr;

	float currentY = 0.0f;
};