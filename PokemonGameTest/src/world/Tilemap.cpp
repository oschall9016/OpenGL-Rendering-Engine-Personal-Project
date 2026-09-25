#include "Tilemap.h"
#include "Tile.h"
#include "Warp.h"

#include <array>
#include <iostream>
#include <cstdint> 

Tilemap::Tilemap(std::vector<Tile> tileMap, float x, float z)
{
	mapTiles = tileMap;
	mapXSize = x;
	mapZSize = z;

	heightMap.resize(mapXSize * mapZSize);
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

	heightMap.resize(mapXSize * mapZSize);
}

// TODO: check if tile not found 
Tile& Tilemap::GetTile(float x, float z)
{
	return mapTiles[(z * mapXSize) + x];
}

// TODO: make sure warp index is set when signature is WARP
void Tilemap::SetTileSignature(float x, float z, uint8_t newSig, uint16_t newWarpIndex)
{
	Tile& tile = mapTiles[(z * mapXSize) + x];

	tile.signature = newSig;

	if (newSig & Tile_Type::WARP)
	{
		tile.warpIndex = newWarpIndex;
	}

	// cant be walkable and collider at the same time
	if (newSig & Tile_Type::COLLIDER)
	{
		tile.signature &= ~Tile_Type::WALKABLE;
	}
	else if (newSig & Tile_Type::WALKABLE)
	{
		tile.signature &= ~Tile_Type::COLLIDER;
	}

}

void Tilemap::SetTileFlag(float x, float z, bool removeFlag, Tile_Type newType, uint16_t newWarpIndex)
{
	Tile& tile = mapTiles[(z * mapXSize) + x];

	if (removeFlag)
	{
		// make sure the flag is set
		if (tile.signature & newType) // do i need to do this?
		{
			tile.signature &= ~newType;
		}

		if (newType & Tile_Type::WARP)
		{
			tile.warpIndex = NO_WARP;
		}
	}
	else
	{
		tile.signature |= newType;
	}

	if (newType & Tile_Type::WARP)
	{
		tile.warpIndex = newWarpIndex;
	}

	// cant be walkable and collider at the same time
	if (newType & Tile_Type::COLLIDER)
	{
		tile.signature &= ~Tile_Type::WALKABLE;
	}
	else if (newType & Tile_Type::WALKABLE)
	{
		tile.signature &= ~Tile_Type::COLLIDER;
	}
}

// TODO: removal
void Tilemap::SetSlope(float x, float z, bool removeSlope, Rotate_Direction direction)
{
	float tileIndex = (z * mapXSize) + x;
	Tile& tile = mapTiles[tileIndex];

	tile.signature |= Tile_Type::SLOPE;

	slopes.insert({ tileIndex, Slope{ direction } });

	heightMap[tileIndex] += 0.5f;
}

void Tilemap::SetTileHeight(float x, float z, float height)
{
	heightMap[(z * mapXSize) + x] = height;
}

float Tilemap::GetTileHeight(float x, float z)
{
	return heightMap[(z * mapXSize) + x];
}