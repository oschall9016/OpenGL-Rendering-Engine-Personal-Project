#include "system_player_moveRenderPosition.h"

#include "EntityComponentSystem.h"

#include "component_player_currentState.h"
#include "component_tilemapPosition.h"
#include "component_Transform.h"

#include "Camera.h"

#include <glm/glm.hpp>

#include <cmath>
#include <iostream>

/*
* Components:
*	- component_player_currentState
*	- component_tilemapPosition
*	- component_Transform
*/

system_player_moveRenderPosition::system_player_moveRenderPosition(EntityComponentSystem& ecs) : ecs(ecs) {}

void system_player_moveRenderPosition::Update(float dt, Camera& camera)
{

	for (auto& entity : entities)
	{
		auto tilePosition = ecs.GetComponent<component_tilemapPosition>(entity);
		auto transform = ecs.GetComponent<component_Transform>(entity);
		auto currentState = ecs.GetComponent<component_player_currentState>(entity);
	
		// calculate amount of distance to move this frame
		float totalDistance = 1.0f; // world unit

		float totalTime = currentState->moveSpeed;

		float speed = totalDistance / totalTime;
		float stepSize = speed * dt;

		auto& tileX = tilePosition->currentX;
		auto& tileZ = tilePosition->currentZ;

		auto tileY = tilePosition->currentY;

		auto& renderX = transform->position.x;
		auto& renderZ = transform->position.z;

		auto& renderY = transform->position.y;

		auto cPosition = camera.GetPosition();
		float cameraZOffset = cPosition.z - renderZ;
		// player warped so teleport position
		if (currentState->warping)
		{
			
			// isolate camera follow distance
			

			// teleport billboard
			renderX = tileX;
			renderZ = tileZ;

			// teleport camera
			cPosition.x = renderX;
			cPosition.z = tileZ + cameraZOffset;
			camera.SetPosition(cPosition);

			currentState->movementVisualsSynced = true;

			continue;
		}

		// horizontal movement

		// right
		if (tileX > renderX)
		{
			renderX = std::min(renderX + stepSize, tileX);
			camera.SetPosition(renderX, cPosition.y, cPosition.z); // TODO: replace with cPosition
		}

		// left
		else if (tileX < renderX)
		{
			renderX = std::max(renderX - stepSize, tileX);
			camera.SetPosition(renderX, cPosition.y, cPosition.z);
		}

		// down
		else if (tileZ > renderZ)
		{
			renderZ = std::min(renderZ + stepSize, tileZ);
			camera.SetPosition(cPosition.x, cPosition.y,  renderZ + cameraZOffset);
		}

		// up
		else if (tileZ < renderZ)
		{
			renderZ = std::max(renderZ - stepSize, tileZ);
			camera.SetPosition(cPosition.x, cPosition.y, renderZ + cameraZOffset);
		}

		// vertical movement
		float cameraYOffset = cPosition.y - renderY;
		cPosition = camera.GetPosition();

		if (tileY > renderY)
		{
			renderY = std::min(renderY + stepSize, tileY);

			camera.SetPosition(cPosition.x,renderY + cameraYOffset,cPosition.z);
		}
		else if (tileY < renderY)
		{
			renderY = std::max(renderY - stepSize, tileY);

			camera.SetPosition(cPosition.x, renderY + cameraYOffset, cPosition.z);
		}

		// lock input until visuals and gameplay are synced
		if (tileX == renderX && tileZ == renderZ)
		{
			currentState->movementVisualsSynced = true;
			currentState->inputLocked = false;
		}
		else
		{
			currentState->movementVisualsSynced = false;
			currentState->inputLocked = true;
		}

		std::cout << "\n";
		std::cout << "[ " << transform->position.x << " , " << transform->position.y << " , " << transform->position.z << " ]";
		std::cout << "\n";

	}
}