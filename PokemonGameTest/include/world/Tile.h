#pragma once

class GameMap;

constexpr int TILE_SIZE = 16;
constexpr int NO_WARP = -1;

// dictates tile behaivor
enum TileSignature : char
{
	WALKABLE = 1 << 0,
	COLLIDER = 1 << 1,
	WARP = 1 << 2
};

struct Tile
{
	TileSignature signature = WALKABLE;
	int warpIndex = NO_WARP;
};
