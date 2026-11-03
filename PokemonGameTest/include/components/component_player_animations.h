#pragma once

#include "OverworldSpriteMovementAnimation.h"

#include "component_player_currentState.h"

struct component_player_animations
{
	OverworldSpriteMovementAnimation movAnimation;

	Input_Action SpriteAction;
	Input_Direction SpriteDirection;
};