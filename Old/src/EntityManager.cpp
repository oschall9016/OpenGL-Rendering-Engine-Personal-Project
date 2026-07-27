#include "EntityManager.h"
#include "Entity.h"

#include <queue>
#include <iostream>
#include <unordered_map>

EntityManager::EntityManager()
{
	livingEntities = 0;
	

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		availableEntities.push(i);
	}
}

Entity EntityManager::CreateEntity()
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

	signatures.insert({ id, {} });

	return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
	if (livingEntitiesSet[entity] == 0) // kind of redundant
	{
		std::cout << "Entity Not Deleted: Entity Does Not Exist\n";
		return;
	}
	signatures.erase(entity);
	availableEntities.push(entity);
	livingEntitiesSet.reset(entity);
	livingEntities--;
}

bool EntityManager::EntityExists(Entity entity)
{
	if (livingEntitiesSet[entity] == 1) return true;
	else return false;
}

void EntityManager::SetSignature(Entity entity, Signature signature)
{
	if (!EntityExists(entity))
	{
		return;
	}
	signatures.at(entity) = signature;
}

Signature EntityManager::GetSignature(Entity entity)
{
	if (!EntityExists(entity))
	{
		return {};
	}

	return signatures.at(entity);
}

