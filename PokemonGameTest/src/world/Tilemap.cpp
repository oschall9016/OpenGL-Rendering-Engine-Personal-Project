#include "Tilemap.h"
#include "Tile.h"
#include "Warp.h"

#include <array>
#include <iostream>
#include <cstdint> 

Tilemap::Tilemap(std::vector<Tile> tileMap, int x, int z)
{
	mapTiles = tileMap;
	mapXSize = x;
	mapZSize = z;

	heightMap.resize(mapXSize * mapZSize);
}

Tilemap::Tilemap(int x, int z)
{
	mapXSize = x;
	mapZSize = z;

	// fills map with walkable tiles
	for (int i = 0; i < x * z; i++)
	{
		Tile newTile;
		mapTiles.push_back(newTile);
	}

	heightMap.resize(mapXSize * mapZSize);
}

// TODO: check if tile not found 
Tile& Tilemap::GetTile(int x, int z)
{
	return mapTiles[(z * mapXSize) + x];
}

// TODO: removal of slope
void Tilemap::SetSlope(int x, int z, bool removeSlope, Rotate_Direction direction)
{
	int tileIndex = (z * mapXSize) + x;
	Tile& tile = mapTiles[tileIndex];

	tile.signature |= Tile_Type::SLOPE;

	slopes.insert({ tileIndex, Slope{ direction } });

	heightMap[tileIndex] += 0.5f;
}

void Tilemap::SetCollider(int x, int z, bool removeCollider)
{
	Tile& tile = mapTiles[(z * mapXSize) + x];

	if (removeCollider)
	{
		tile.signature &= ~Tile_Type::COLLIDER;
	}
	else
	{
		tile.signature |= Tile_Type::COLLIDER;
	}

}

void  Tilemap::SetWarp(int x, int z, bool removeWarp, uint16_t warpIndex)
{
	int tileIndex = (z * mapXSize) + x;
	Tile& tile = mapTiles[tileIndex];

	if (removeWarp)
	{
		tile.signature &= ~Tile_Type::WARP;
		tile.warpIndex = NO_WARP;
	}
	else
	{
		tile.signature |= Tile_Type::WARP;
		tile.warpIndex = warpIndex;
	}
}

void Tilemap::SetTileHeight(int x, int z, float height)
{
	heightMap[(z * mapXSize) + x] = height;
}

void Tilemap::SetTileTextureIndex(int x, int z, float texIndex)
{
	int tileIndex = (z * mapXSize) + x;
	Tile& tile = mapTiles[tileIndex];

	tile.textureIndex = texIndex;
}

float Tilemap::GetTileHeight(int x, int z)
{
	return heightMap[(z * mapXSize) + x];
}