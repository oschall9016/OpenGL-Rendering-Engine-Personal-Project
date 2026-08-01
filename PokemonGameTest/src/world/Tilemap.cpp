#include "Tilemap.h"
#include "Tile.h"

#include <array>
#include <iostream>

Tilemap::Tilemap(std::vector<Tile> tileMap, int rows, int cols)
{
	map = tileMap;
	mapRows = rows;
	mapCols = cols;
}

Tilemap::Tilemap(int rows, int cols)
{
	mapRows = rows;
	mapCols = cols;

	// fills map with walkable tiles
	for (int i = 0; i < rows * cols; i++)
	{
		Tile newTile;
		map.push_back(newTile);
	}
}

 // TODO: check if tile not found 
const Tile& Tilemap::GetTile(int row, int col)
{
	return map[(row * mapCols) + col];
}

void Tilemap::SetTileSignature(int row, int col, TileSignature newsig)
{
	Tile& tile = map[(row * mapCols) + col];
	tile.signature = newsig;
}

// debug
void Tilemap::PrintTilemap()
{
	for (int i = 0; i < mapRows; i++)
	{
		std::cout << "\n";

		for (int j = 0; j < mapCols; j++)
		{
			Tile tile = GetTile(i, j);

			if (tile.signature == WALKABLE) std::cout << "0";
			else std::cout << "1";

			std::cout << " ";
		}
	}
	std::cout << "\n";
}