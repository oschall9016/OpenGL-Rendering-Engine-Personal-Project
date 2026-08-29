#include "Tilemap.h"
#include "Tile.h"

#include <array>
#include <iostream>

Tilemap::Tilemap(std::vector<Tile> tileMap, int x, int z)
{
	map = tileMap;
	mapXSize = x;
	mapZSize = z;
}

Tilemap::Tilemap(int x, int z)
{
	mapXSize = x;
	mapZSize = z;

	// fills map with walkable tiles
	for (int i = 0; i < x * z; i++)
	{
		Tile newTile;
		map.push_back(newTile);
	}
}

 // TODO: check if tile not found 
Tile& Tilemap::GetTile(int x, int z)
{
	return map[(z * mapXSize) + x];
}

void Tilemap::SetTileSignature(int x, int z, TileSignature newsig)
{
	Tile& tile = map[(z * mapXSize) + x];
	tile.signature = newsig;
}

// debug
void Tilemap::PrintTilemap()
{
	for (int i = 0; i < mapZSize; i++)
	{
		std::cout << "\n";

		for (int j = 0; j < mapXSize; j++)
		{
			Tile tile = GetTile(i, j);

			if (tile.signature == WALKABLE) std::cout << "0";
			else std::cout << "1";

			std::cout << " ";
		}
	}
	std::cout << "\n";
}