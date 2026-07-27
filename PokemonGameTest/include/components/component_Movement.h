#pragma once

#include "OverworldSpriteMovementAnimation.h"

struct component_Movement
{
	Action currentAction = Standing;
	Direction currentDirection = Down;

	OverworldSpriteMovementAnimation animation;
};
