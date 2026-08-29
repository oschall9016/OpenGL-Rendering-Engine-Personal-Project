#pragma once

#include <vector>

#include "Tile.h"

class Tilemap
{
public:
	Tilemap(std::vector<Tile> map, int x, int z);
	Tilemap(int x, int z);

	Tile& GetTile(int x, int z);
	void SetTileSignature(int x, int z, TileSignature newsig);

	int mapXSize, mapZSize;

	// debug
	void PrintTilemap();
	
private:
	std::vector<Tile> map;
	
};