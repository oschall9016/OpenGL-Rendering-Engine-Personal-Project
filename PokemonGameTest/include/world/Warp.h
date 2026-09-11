#pragma once

class GameMap;

enum class Exit_Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct WarpDestination
{
	GameMap* gameMap = nullptr;

	float x = 0;
	float z = 0;

	Exit_Direction exitDirection = Exit_Direction::DOWN;
};

struct Warp
{
	WarpDestination destinations[2];

	int currentIndex = 0; // swap with 1 - destination
};
