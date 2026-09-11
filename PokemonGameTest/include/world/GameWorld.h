#pragma once

#include "Tile.h"
#include "GameMap.h"
#include "Tilemap.h"

#include "Renderer.h"
#include "Camera.h"

#include <vector>
#include <memory>

class GameWorld
{
public:
	GameWorld();

	GameMap* currentMap = nullptr;

	std::vector<GameMap*> gameMaps = {};

	void DrawCurrentMap(Renderer& renderer, Camera& camera);
private:

	// ecs here eventually

	
};