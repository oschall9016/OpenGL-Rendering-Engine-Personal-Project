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

		float holdCooldown = 0; // move speed shouldn't go under this value or it stutters currentState->moveSpeed / 2

		// input is only pressed, update immediately 
		if (input.isKeyPressed(SDL_SCANCODE_W))
		{
			updateComponent(currentState, Input_Direction::UP);
			currentState->keyState = Key_State::PRESSED;
		}
		else if (input.isKeyPressed(SDL_SCANCODE_S))
		{
			updateComponent(currentState, Input_Direction::DOWN);
			currentState->keyState = Key_State::PRESSED;
		}
		else if (input.isKeyPressed(SDL_SCANCODE_A))
		{
			updateComponent(currentState, Input_Direction::LEFT);
			currentState->keyState = Key_State::PRESSED;
		}
		else if (input.isKeyPressed(SDL_SCANCODE_D))
		{
			updateComponent(currentState, Input_Direction::RIGHT);
			currentState->keyState = Key_State::PRESSED;
		}

		// input is being held
		// only update after a small delay (prevents double stepping on press)
		// and if the movement visuals are synced (prevents moving one tile too far)
		else if (input.isKeyHeld(SDL_SCANCODE_W))
		{
			currentState->keyState = Key_State::HELD;
			if (holdTimer >= holdCooldown && currentState->movementVisualsSynced)
			{
				updateComponent(currentState, Input_Direction::UP);
				holdTimer = 0.0f;
			}
			else holdTimer += dt;
		}
		else if (input.isKeyHeld(SDL_SCANCODE_S))
		{
			currentState->keyState = Key_State::HELD;
			if (holdTimer >= holdCooldown && currentState->movementVisualsSynced)
			{
				updateComponent(currentState, Input_Direction::DOWN);
				holdTimer = 0.0f;
			}
			else holdTimer += dt;
		}
		else if (input.isKeyHeld(SDL_SCANCODE_A))
		{
			currentState->keyState = Key_State::HELD;
			if (holdTimer >= holdCooldown && currentState->movementVisualsSynced)
			{
				updateComponent(currentState, Input_Direction::LEFT);
				holdTimer = 0.0f;
			}
			else holdTimer += dt;
		}
		else if (input.isKeyHeld(SDL_SCANCODE_D))
		{
			currentState->keyState = Key_State::HELD;
			if (holdTimer >= holdCooldown && currentState->movementVisualsSynced)
			{
				updateComponent(currentState, Input_Direction::RIGHT);
				holdTimer = 0.0f;
			}
			else holdTimer += dt;
		}
		else
		{
			holdTimer = 0.0f;
			currentState->currentAction = Input_Action::STANDING;
			currentState->keyState = Key_State::RELEASED;
		}
	}
}

void system_player_inputDetection::updateComponent(component_player_currentState* state, Input_Direction direction)
{
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

	state->moveRequest = true;
	state->currentDirection = direction;
}
