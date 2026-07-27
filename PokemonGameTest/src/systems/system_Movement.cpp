#include "system_Movement.h"

#include "SDLInput.h"
#include "EntityComponentSystem.h"

#include "component_Movement.h"
#include "component_Model.h"


system_Movement::system_Movement(EntityComponentSystem& ecs, SDLInput& input) : ecs(ecs), input(input) {}

void system_Movement::Update()
{
	for (auto& entity : entities)
	{
		auto* movement = ecs.GetComponent<component_Movement>(entity);
		auto* visuals = ecs.GetComponent<component_Model>(entity);

		int row, col;
		
		if (input.isKeyPressed(SDL_SCANCODE_W)) UpdateMovementAnim(movement, Up);
			
		else if (input.isKeyPressed(SDL_SCANCODE_S)) UpdateMovementAnim(movement, Down);

		else if (input.isKeyPressed(SDL_SCANCODE_A)) UpdateMovementAnim(movement, Left);

		else if (input.isKeyPressed(SDL_SCANCODE_D)) UpdateMovementAnim(movement, Right);
		
		else if(movement->animation.GetCurrentFrame() != 0) // makes sure animation is on stationary frame when not moving
		{
			movement->animation.Update();
			movement->currentAction = Walking;
		}
		else // prevents running animation when standing still
		{
			movement->currentAction = Walking;
		}

		row = movement->currentAction;
		col = movement->currentDirection + movement->animation.GetCurrentFrame(); // TODO: make animation not a copy

		visuals->shader->use();

		visuals->shader->setFloat("uvRow", (float)row);
		visuals->shader->setFloat("uvCol", (float)col);
		
	}
}

void system_Movement::UpdateMovementAnim(component_Movement* movement, Direction movDirection)
{
	if (input.isKeyPressed(SDL_SCANCODE_LSHIFT)) movement->currentAction = Running;
	else movement->currentAction = Walking;

	movement->currentDirection = movDirection;
	movement->animation.Update();
}