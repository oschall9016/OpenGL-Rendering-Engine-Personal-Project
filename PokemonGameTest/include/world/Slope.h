#pragma once

enum class Rotate_Direction
{
	UP = 180,
	DOWN = 0,
	LEFT = -90,
	RIGHT = 90

};

struct Slope
{
	Rotate_Direction direction;
};

// maybe seperate set functions