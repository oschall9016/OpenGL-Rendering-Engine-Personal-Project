#pragma once

#include <vector>

#include "Tile.h"

class Tilemap
{
public:
	Tilemap(std::vector<Tile> map, int rows, int cols);
	Tilemap(int rows, int cols);

	const Tile& GetTile(int row, int col);
	void SetTileSignature(int row, int col, TileSignature newsig);

	int mapRows, mapCols;

	// debug
	void PrintTilemap();
	
private:
	std::vector<Tile> map;
	
};