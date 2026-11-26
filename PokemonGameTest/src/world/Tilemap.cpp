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
 
Tile* Tilemap::GetTile(int x, int z)
{
	if (x < 0 || z < 0 || x >= mapXSize || z >= mapZSize)
	{
		std::cout << "Could Not Get Tile: [" << x << ", " << z << "], Tile Not Found \n";
		return nullptr;
	}

	int tileIndex = (z * mapXSize) + x;
	return &mapTiles[tileIndex];
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

int Tilemap::GetMapXSize()
{
	return mapXSize;
}

int Tilemap::GetMapZSize()
{
	return mapZSize;
}

const Slope* Tilemap::GetSlope(int index)
{
	auto slopePair = slopes.find(index);

	if (slopePair == slopes.end())
	{
		std::cout << "Cannot Get Slope At Tile Index: " << index << ", Slope Does Not Exist\n";
		return nullptr;
	}

	return &slopePair->second;
}

size_t Tilemap::GetSlopeListSize()
{
	return slopes.size();
}