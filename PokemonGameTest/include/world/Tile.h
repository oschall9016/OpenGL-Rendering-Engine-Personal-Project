#pragma once

#include <cstdint>

class GameMap;

constexpr int TILE_SIZE = 16; // 16 pixels, 1 world unit
constexpr int NO_WARP = -1;

// dictates tile behaivor
enum Tile_Type : uint8_t
{
	WALKABLE = 1 << 0,
	COLLIDER = 1 << 1,
	WARP = 1 << 2,
	SLOPE = 1 << 3
};

struct Tile
{
	uint16_t warpIndex = NO_WARP;
	uint8_t signature = Tile_Type::WALKABLE;
};
