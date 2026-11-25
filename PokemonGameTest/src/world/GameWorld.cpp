#include "GameWorld.h"


#include "Renderer.h"
#include "Camera.h"

GameWorld::GameWorld() {}

GameMap* GameWorld::CreateGameMap(std::string mapName, float TileXSize, float TileZSize, std::shared_ptr<Texture> tileSheet, std::string mapPath)
{
	Tilemap tilemap(TileXSize, TileZSize);
	GameMap gameMap(mapName, tilemap, tileSheet, mapPath);

	gameMaps.insert({ mapName, gameMap });

	return GetMap(mapName);
}

GameMap* GameWorld::GetCurrentMap()
{
	return currentMap;
}

GameMap* GameWorld::GetMap(std::string mapName)
{
	return &gameMaps.at(mapName);
}

Warp* GameWorld::GetWarp(int index)
{
	return &mapWarps[index];
}

void GameWorld::SetCurrentMap(std::string mapName)
{
	currentMap = &gameMaps.at(mapName);
}

void GameWorld::AddWarp(Warp warp)
{
	mapWarps.push_back(warp);
}

void GameWorld::DrawCurrentMap(Renderer& renderer, Camera& camera)
{
	currentMap->DrawGameMap(renderer, camera);
}