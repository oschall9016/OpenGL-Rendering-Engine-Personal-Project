#pragma once
#include "EntityHandler.h"
#include "Entity.h"

#include <queue>
#include <iostream>

EntityHandler::EntityHandler()
{
	livingEntities = 0;
	

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		availableEntities.push(i);
	}
}

Entity EntityHandler::CreateEntity()
{

	// TODO: better error handling
	if (livingEntities == MAX_ENTITIES)
	{
		std::cout << "Entity Not Created: Max Entity Count Reached\n";
		return -1;
	}

	Entity id = availableEntities.front();
	availableEntities.pop();
	livingEntitiesSet.set(id);
	livingEntities++;
	return id;
}

void EntityHandler::DestroyEntity(Entity entity)
{
	if (livingEntitiesSet[entity] == 0) // kind of redundant
	{
		std::cout << "Entity Not Deleted: Entity Does Not Exist\n";
		return;
	}
	availableEntities.push(entity);
	livingEntitiesSet.reset(entity);
	livingEntities--;
}

bool EntityHandler::DoesEntityExist(Entity entity)
{
	if (livingEntitiesSet[entity] == 1) return true;
	else return false;
}

