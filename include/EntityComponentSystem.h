#pragma once

// Sparse Set Entity Component Sytstem

// a full architype ecs design might be overkill for this project's
// goals but would be a fun future project for a larger more efficient engine

#include "Entity.h"
#include "EntityHandler.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "SparseSet.h"

#include <iostream>

class EntityComponentSystem
{
public:
	EntityComponentSystem(); // constructor

	Entity CreateEntity(); // register an entity with the EntityHandler

	void DestroyEntity(Entity entity); // destroy entity and clear its data

	template <typename ComponentType>
	void RegisterComponent(); // register a component with the ComponentManager

	template <typename ComponentType>
	void AddComponent(Entity entity, ComponentType component); // store a component for an entity in the ComponentManager

	template <typename ComponentType>
	void RemoveComponent(Entity entity); // removes a component for an entity in the ComponentManager

	template <typename ComponentType>
	ComponentType* GetComponent(Entity entity); // get a pointer to an entities component from the ComponentManager

	template <typename ComponentType>
	std::shared_ptr<SparseSet<ComponentType>> GetComponentSet(); // get the specific SparseSet of a USS stored in componentArrays. TODO: remove this function 

	template <typename SystemName, typename... ComponentTypes>
	std::shared_ptr<SystemName> RegisterSystem(std::shared_ptr<SystemName> system); // register a system for use with the SystemManager


private:
	EntityHandler entityHandler;
	ComponentManager componentManager;
	SystemManager systemManager;
};

inline EntityComponentSystem::EntityComponentSystem() : entityHandler(), componentManager()
{
}

inline Entity EntityComponentSystem::CreateEntity()
{
	return entityHandler.CreateEntity();
}

inline void EntityComponentSystem::DestroyEntity(Entity entity)
{
	if (entityHandler.EntityExists(entity))
	{
		entityHandler.DestroyEntity(entity);
		componentManager.EntityDeleted(entity);
	}
}

template <typename ComponentType>
void EntityComponentSystem::RegisterComponent()
{
	componentManager.RegisterComponent<ComponentType>();
}

template <typename ComponentType>
void EntityComponentSystem::AddComponent(Entity entity, ComponentType component)
{
	if (!entityHandler.EntityExists(entity))
	{
		return;
	}

	componentManager.AddComponent<ComponentType>(entity, component);

	// update entity signature
	Signature entitySig = entityHandler.GetSignature(entity);
	int bitPosition = componentManager.GetBitPosition<ComponentType>();

	entitySig.set(bitPosition);

	entityHandler.SetSignature(entity, entitySig);

	systemManager.EntitySignatureChanged(entity, entitySig);
}

template <typename ComponentType>
void EntityComponentSystem::RemoveComponent(Entity entity)
{
	if (entityHandler.EntityExists(entity))
	{
		return;
	}

	componentManager.RemoveComponent<ComponentType>(entity);

	// update entity signature
	Signature entitySig = entityHandler.GetSignature(entity);
	int bitPosition = componentManager.GetBitPosition<ComponentType>();

	entitySig.reset(bitPosition);

	entityHandler.SetSignature(entity, entitySig);

	systemManager.EntitySignatureChanged(entity, entitySig);
}

template <typename ComponentType>
ComponentType* EntityComponentSystem::GetComponent(Entity entity)
{
	if (!entityHandler.EntityExists(entity))
	{
		return nullptr;
	}

	return componentManager.GetComponent<ComponentType>(entity);
}

template <typename ComponentType>
std::shared_ptr<SparseSet<ComponentType>> EntityComponentSystem::GetComponentSet()
{
	return componentManager.GetComponentSet<ComponentType>();
} 

template <typename SystemName, typename... ComponentTypes>
std::shared_ptr<SystemName> EntityComponentSystem::RegisterSystem(std::shared_ptr<SystemName> system)
{
	Signature systemSignature;

	// set signature by folding over each of the given components
	(systemSignature.set(componentManager.GetBitPosition<ComponentTypes>()), ...);

	return systemManager.RegisterSystem<SystemName>(systemSignature, system);
}