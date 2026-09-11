#pragma once

#include <vector>

#include "Tile.h"
#include "Warp.h"

class Tilemap
{
public:
	Tilemap(std::vector<Tile> mapTiles, float x, float z);
	Tilemap(float x, float z);

	Tile& GetTile(float x, float z);
	void SetTileSignature(float x, float z, TileSignature newsig, int newWarpIndex = NO_WARP);

	int mapXSize, mapZSize;

	// debug
	void PrintTilemap();
	
	std::vector<Tile> mapTiles;
	std::vector<Warp*> mapWarps = {};

private:

	
};