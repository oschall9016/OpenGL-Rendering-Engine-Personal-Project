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

		float totalDistance = 1.0f;
		float totalTime = currentState->moveSpeed; // need to catch up before the next movement event

		float speed = totalDistance / totalTime;
		float stepSize = speed * dt;

		auto& tileX = tilePosition->currentX;
		auto& tileZ = tilePosition->currentZ;

		auto& renderX = transform->position.x;
		auto& renderZ = transform->position.z;

		if (tileX > renderX)
		{
			renderX = std::min(renderX + stepSize, tileX);
			camera.SetPosition(renderX, camera.GetPosition().y, camera.GetPosition().z);
		}
		else if (tileX < renderX)
		{
			renderX = std::max(renderX - stepSize, tileX);
			camera.SetPosition(renderX, camera.GetPosition().y, camera.GetPosition().z);
		}
		else if (tileZ > renderZ)
		{
			camera.SetPosition(camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z + std::min(stepSize, tileZ - renderZ));
			renderZ = std::min(renderZ + stepSize, tileZ);
			
		}
		else if (tileZ < renderZ)
		{
			camera.SetPosition(camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z - std::min(stepSize, renderZ - tileZ));
			renderZ = std::max(renderZ - stepSize, tileZ);
		}

		if (tileX == renderX && tileZ == renderZ)
		{
			currentState->movementVisualsSynced = true;
		}
		else
		{
			currentState->movementVisualsSynced = false;
		}

	}
}