#pragma once

#include <queue>
#include "Entity.h"

#include <bitset>
#include <vector>

class EntityHandler
{
public:
	EntityHandler(int maxEntityAmount);
	Entity CreateEntity();
	void DeleteEntity(Entity entity);

	void SetSignature(Entity entity, Signature signature);
	Signature GetSignature(Entity entity);
private:
	std::queue<Entity> availableEntities;
	std::vector<Signature> signatures;

	int MAX_ENTITIES; // TODO: look back through project at overall const styling
	int livingEntities;
};