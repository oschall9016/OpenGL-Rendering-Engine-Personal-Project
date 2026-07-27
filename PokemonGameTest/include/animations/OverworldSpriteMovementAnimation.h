#pragma once

#include "SpriteAnimation.h"

// speed the character is moving
// determines the row of the sprite sheet
enum Action
{
	Standing = 0, // stationary sprites are just the stationary walk sprites
	Walking = 0,
	Running = 1
};

// direction the sprite is facing
// determines which 3 frame animation to play (4 per row)
enum Direction
{
	Up = 0,
	Down = 3,
	Left = 6,
	Right = 9
};

// uv row = Action
// uv col = Direction + Frame

class OverworldSpriteMovementAnimation : public SpriteAnimation
{
public:
	OverworldSpriteMovementAnimation();
};