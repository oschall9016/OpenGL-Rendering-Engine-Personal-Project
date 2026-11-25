#pragma once

#include "Tile.h"
#include "Warp.h"
#include "GameMap.h"
#include "Tilemap.h"

#include "Renderer.h"
#include "Camera.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

class GameWorld
{
public:
	GameWorld();

	//GameMap* currentMap = nullptr;
	//std::vector<GameMap*> gameMaps = {};

	GameMap* CreateGameMap(std::string mapName, int TileXSize, int TileZSize, std::shared_ptr<Texture> tileSheet, std::string mapPath);

	GameMap* GetCurrentMap();
	GameMap* GetMap(std::string mapName);
	Warp* GetWarp(int index);

	void SetCurrentMap(std::string mapName);
	void AddWarp(Warp warp);

	void DrawCurrentMap(Renderer& renderer, Camera& camera);
private:

	GameMap* currentMap = nullptr;
	std::unordered_map<std::string, GameMap> gameMaps = {};
	std::vector<Warp> mapWarps = {};
};