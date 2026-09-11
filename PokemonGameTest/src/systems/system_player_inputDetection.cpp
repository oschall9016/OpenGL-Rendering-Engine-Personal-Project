#include "system_player_inputDetection.h"

#include "EntityComponentSystem.h"
#include "SDLInput.h"

#include "component_player_currentState.h"

#include <iostream>

/*
* Components:
*	- component_player_currentState
*/

system_player_inputDetection::system_player_inputDetection(EntityComponentSystem& ecs, SDLInput& input) : ecs(ecs), input(input) {}

void system_player_inputDetection::Update(float dt)
{

	for (auto& entity : entities)
	{
		auto currentState = ecs.GetComponent<component_player_currentState>(entity);

		if (currentState->inputLocked)
		{
			continue;
		}

		// input is only pressed, update immediately 
		if (input.isKeyPressed(SDL_SCANCODE_W))
		{
			updateComponent(currentState, Input_Direction::UP, Key_State::PRESSED);
		}
		else if (input.isKeyPressed(SDL_SCANCODE_S))
		{
			updateComponent(currentState, Input_Direction::DOWN, Key_State::PRESSED);
		}
		else if (input.isKeyPressed(SDL_SCANCODE_A))
		{
			updateComponent(currentState, Input_Direction::LEFT, Key_State::PRESSED);
		}
		else if (input.isKeyPressed(SDL_SCANCODE_D))
		{
			updateComponent(currentState, Input_Direction::RIGHT, Key_State::PRESSED);
		}

		// input is being held
		else if (input.isKeyHeld(SDL_SCANCODE_W))
		{
			updateComponent(currentState, Input_Direction::UP, Key_State::HELD);
		}
		else if (input.isKeyHeld(SDL_SCANCODE_S))
		{
			updateComponent(currentState, Input_Direction::DOWN, Key_State::HELD);
		}
		else if (input.isKeyHeld(SDL_SCANCODE_A))
		{
			updateComponent(currentState, Input_Direction::LEFT, Key_State::HELD);
		}
		else if (input.isKeyHeld(SDL_SCANCODE_D))
		{
			updateComponent(currentState, Input_Direction::RIGHT, Key_State::HELD);
		}

		// no input
		else
		{
			currentState->currentAction = Input_Action::STANDING;
			currentState->keyState = Key_State::RELEASED;
		}
	}
}

void system_player_inputDetection::updateComponent(component_player_currentState* state, Input_Direction direction, Key_State keyState)
{
	// player is running if shift is held
	if (input.isKeyPressed(SDL_SCANCODE_LSHIFT) || input.isKeyHeld(SDL_SCANCODE_LSHIFT))
	{
		state->currentAction = Input_Action::RUNNING;
		state->moveSpeed = RUN_SPEED;
	}
	else
	{
		state->currentAction = Input_Action::WALKING;
		state->moveSpeed = WALK_SPEED;
	}

	state->currentDirection = direction;
	state->keyState = keyState;

	// attempting to move
	state->moveRequest = true;
	
}
