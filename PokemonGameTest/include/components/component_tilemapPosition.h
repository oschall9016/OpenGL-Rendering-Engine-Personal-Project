#pragma once

#include "Tilemap.h"

struct component_tilemapPosition
{
	float currentX = 0.0f;
	float currentZ = 0.0f;

	Tilemap* currentMap = nullptr;
};