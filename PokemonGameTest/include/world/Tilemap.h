#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint> 

#include "Tile.h"
#include "Slope.h"

class Tilemap
{
public:
	Tilemap(std::vector<Tile> mapTiles, int x, int z);
	Tilemap(int x, int z);

	Tile* GetTile(int x, int z);
	float GetTileHeight(int x, int z);

	void SetTileHeight(int x, int z, float height);

	void SetTileTextureIndex(int x, int z, float texIndex);

	void SetSlope(int x, int z, bool removeSlope, Rotate_Direction direction = Rotate_Direction::UP);
	void SetCollider(int x, int z, bool removeCollider);
	void SetWarp(int x, int z, bool removeWarp, uint16_t warpIndex);

	int GetMapXSize();
	int GetMapZSize();

	const Slope* GetSlope(int index);
	size_t GetSlopeListSize();
	
private:
	int mapXSize, mapZSize;

	std::vector<Tile> mapTiles;
	std::vector<float> heightMap;

	std::unordered_map<int, Slope> slopes = {};
};