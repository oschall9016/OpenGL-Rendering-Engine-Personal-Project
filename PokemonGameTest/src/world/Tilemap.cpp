#include "Tilemap.h"
#include "Tile.h"
#include "Warp.h"

#include <array>
#include <iostream>

Tilemap::Tilemap(std::vector<Tile> tileMap, float x, float z)
{
	mapTiles = tileMap;
	mapXSize = x;
	mapZSize = z;
}

Tilemap::Tilemap(float x, float z)
{
	mapXSize = x;
	mapZSize = z;

	// fills map with walkable tiles
	for (int i = 0; i < x * z; i++)
	{
		Tile newTile;
		mapTiles.push_back(newTile);
	}
}

 // TODO: check if tile not found 
Tile& Tilemap::GetTile(float x, float z)
{
	return mapTiles[(z * mapXSize) + x];
}

// TODO: make sure warp index is set when signature is WARP
void Tilemap::SetTileSignature(float x, float z, TileSignature newsig, int newWarpIndex)
{
	Tile& tile = mapTiles[(z * mapXSize) + x];
	tile.signature = newsig;
	tile.warpIndex = newWarpIndex;

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

			if (tile.signature & WALKABLE) std::cout << "0";
			else std::cout << "1";

			std::cout << " ";
		}
	}
	std::cout << "\n";
}