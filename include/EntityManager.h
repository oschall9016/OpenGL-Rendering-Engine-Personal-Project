#pragma once

#include <queue>
#include "Entity.h"

#include <bitset>
#include <unordered_map>

class EntityManager
{
public:
	EntityManager();
	
	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	bool EntityExists(Entity entity);

	void SetSignature(Entity entity, Signature signature);
	
	Signature GetSignature(Entity entity);


private:
	std::queue<Entity> availableEntities;
	std::unordered_map<Entity,Signature> signatures;
	std::bitset<MAX_ENTITIES> livingEntitiesSet;

	int livingEntities;
};