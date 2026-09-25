#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint> 

#include "Tile.h"
#include "Warp.h"
#include "Slope.h"

class Tilemap
{
public:
	Tilemap(std::vector<Tile> mapTiles, float x, float z);
	Tilemap(float x, float z);

	Tile& GetTile(float x, float z);
	float GetTileHeight(float x, float z);

	void SetTileSignature(float x, float z, uint8_t newSig, uint16_t newWarpIndex = NO_WARP);
	void SetTileFlag(float x, float z, bool removeFlag, Tile_Type newType, uint16_t newWarpIndex = NO_WARP);
	void SetTileHeight(float x, float z, float height);

	void SetSlope(float x, float z, bool removeSlope, Rotate_Direction direction = Rotate_Direction::UP);
	void SetCollider(float x, float z, bool removeCollider);
	void SetWarp(float x, float z, bool removeWarp);
	

	float mapXSize, mapZSize;
	
	std::vector<Tile> mapTiles;
	std::vector<float> heightMap;

	std::unordered_map<float,Slope> slopes = {};

	std::vector<Warp*> mapWarps = {};

private:

	
};