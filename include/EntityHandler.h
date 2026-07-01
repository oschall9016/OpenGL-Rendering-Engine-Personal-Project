#pragma once

#include <queue>
#include "Entity.h"

#include <bitset>
#include <vector>

class EntityHandler
{
public:
	EntityHandler();
	
	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	bool DoesEntityExist(Entity entity);

	void SetSignature(Entity entity, Signature signature);
	
	Signature GetSignature(Entity entity);


private:
	std::queue<Entity> availableEntities;
	std::vector<Signature> signatures;
	std::bitset<MAX_ENTITIES> livingEntitiesSet;

	int livingEntities;
};