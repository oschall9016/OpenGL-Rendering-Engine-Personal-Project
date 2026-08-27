#include "system_player_playAnimation.h"

#include "component_player_currentState.h"
#include "component_player_animations.h"
#include "component_Model.h"

/*
* Components:
*	- component_player_currentState
*	- component_player_animations
*   - component_Model
*/

system_player_playAnimation::system_player_playAnimation(EntityComponentSystem& ecs) : ecs(ecs) {}

// uv row = Action
// uv col = Direction + Frame

/*
void system_player_playAnimation::Update()
{
	for (auto& entity : entities)
	{
		auto currentState = ecs.GetComponent<component_player_currentState>(entity);
		auto shader = ecs.GetComponent<component_Model>(entity)->shader;
		auto animation = ecs.GetComponent<component_player_animations>(entity);

		
		// cast to int for uv calculating
		int currentAction = static_cast<int>(currentState->currentAction);
		int currentDirection = static_cast<int>(currentState->currentDirection);

		int uvRow = 0;
		int uvCol = 0;

		switch (currentState->currentAction)
		{
		case Sprite_Action::STANDING:
			if (animation->movAnimation.GetCurrentFrame() != 0)
			{
				animation->movAnimation.Update();
			}
			break;
		case Sprite_Action::WALKING:
			animation->movAnimation.Update();
			break;
		case Sprite_Action::RUNNING:
			animation->movAnimation.Update();
			break;
		}

		uvRow = currentAction;
		uvCol = currentDirection + animation->movAnimation.GetCurrentFrame();

		shader->use();

		shader->setFloat("uvRow", (float)uvRow);
		shader->setFloat("uvCol", (float)uvCol);

	}
}
*/

void system_player_playAnimation::Update(float dt)
{
	for (auto& entity : entities)
	{
		auto currentState = ecs.GetComponent<component_player_currentState>(entity);
		auto shader = ecs.GetComponent<component_Model>(entity)->shader;
		auto animation = ecs.GetComponent<component_player_animations>(entity);

		// only update when not moving since input state can change during animation
		if (currentState->movementVisualsSynced)
		{
			animation->SpriteAction = currentState->currentAction;
		}

		// only update animation halfway through a tile transition
		else if (updateCooldown >= (currentState->moveSpeed / 2))
		{
			animation->movAnimation.Update();
			updateCooldown = 0;
		}
		else updateCooldown += dt;

		animation->SpriteDirection = currentState->spriteDirection;

		float uvRow;
		float uvCol;
		
		uvRow = static_cast<float>(animation->SpriteAction);

		// the standing enum has a different value so it can be used to compare against walking but is techically part of the walking animation
		if (animation->SpriteAction == Input_Action::STANDING)
		{
			uvRow = 0;

			// we should be on a standing frame if we arent moving
			if (animation->movAnimation.GetCurrentFrame() != 0 && currentState->movementVisualsSynced)
			{
				animation->movAnimation.Update();
			}
		}

		uvCol = static_cast<float>(animation->SpriteDirection) + animation->movAnimation.GetCurrentFrame();

		shader->use();

		shader->setFloat("uvRow", (float)uvRow);
		shader->setFloat("uvCol", (float)uvCol);

	}
}