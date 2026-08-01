#pragma once

constexpr int TILE_SIZE = 32;

// dictates tile behaivor
enum TileSignature : char
{
	WALKABLE = 1 << 0,
	COLLIDER = 1 << 1
};

struct Tile
{
	TileSignature signature = WALKABLE;
};