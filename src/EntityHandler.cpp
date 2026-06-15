#pragma once
#include "EntityHandler.h"
#include "Entity.h"

#include <queue>
#include <iostream>

EntityHandler::EntityHandler(int maxEntityAmount)
{
	MAX_ENTITIES = maxEntityAmount;
	signatures.resize(MAX_ENTITIES);
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
	livingEntities++;
	return id;
}

void EntityHandler::DeleteEntity(Entity entity)
{
	if (entity >= livingEntities) // TODO: not correctly checking
	{
		std::cout << "Entity Not Deleted: Entity Does Not Exist\n";
		return;
	}
	availableEntities.push(entity);
	livingEntities--;

	signatures[entity].reset();
}

void EntityHandler::SetSignature(Entity entity, Signature signature)
{
	if (entity >= livingEntities) // TODO: not correctly checking
	{
		std::cout << "Signature Not Set: Entity Does Not Exist\n";
		return;
	}
	signatures[entity] = signature;
}

Signature EntityHandler::GetSignature(Entity entity)
{
	if (entity >= livingEntities)
	{
		std::cout << "Signature Not Set: Entity Does Not Exist\n";
		return Signature(); //TODO error handling still sucks
	}
	return signatures[entity];
}

