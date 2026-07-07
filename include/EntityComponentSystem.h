#pragma once

// Sparse Set Entity Component Sytstem
// 
// good cache locallity when systems use one component
// but because each component's sparse set has a different order
// multiple components in one system won't be perfectly lined up
// so lookups and extra memory accesses will be needed

// for maximum efficiency try to keep the necessary data packaged
// in one component

// a full architype ecs design might be overkill for this project's
// goals but would be a fun future project for a heftier engine

// TODO: change overall template names from T to something more descriptive

#include "Entity.h"
#include "EntityHandler.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "SparseSet.h"

#include <iostream>

class EntityComponentSystem
{
public:
	EntityComponentSystem();

	Entity CreateEntity();

	void DestroyEntity(Entity entity);

	template <typename T>
	void RegisterComponent();

	template <typename T>
	void AddComponent(Entity entity, T component);

	template <typename T>
	void RemoveComponent(Entity entity);

	template <typename T>
	T* GetComponent(Entity entity);

	template <typename T>
	std::shared_ptr<SparseSet<T>> GetComponentSet();

	template <typename SystemName, typename... ComponentNames>
	std::shared_ptr<SystemName> RegisterSystem(std::shared_ptr<SystemName> system);


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
	if (entityHandler.DoesEntityExist(entity))
	{
		entityHandler.DestroyEntity(entity);
		componentManager.EntityDeleted(entity);
	}
}

template <typename T>
void EntityComponentSystem::RegisterComponent()
{
	componentManager.RegisterComponent<T>();
}

template <typename T>
void EntityComponentSystem::AddComponent(Entity entity, T component)
{
	if (!entityHandler.DoesEntityExist(entity)) // TODO: change name to EntityExists
	{
		return;
	}

	componentManager.AddComponent<T>(entity, component);

	// update entity signature
	Signature entitySig = entityHandler.GetSignature(entity);
	int bitPosition = componentManager.GetBitPosition<T>();

	entitySig.set(bitPosition);

	entityHandler.SetSignature(entity, entitySig);

	systemManager.EntitySignatureChanged(entity, entitySig);
}

template <typename T>
void EntityComponentSystem::RemoveComponent(Entity entity)
{
	if (entityHandler.DoesEntityExist(entity))
	{
		return;
	}

	componentManager.RemoveComponent<T>(entity);

	// update entity signature
	Signature entitySig = entityHandler.GetSignature(entity);
	int bitPosition = componentManager.GetBitPosition<T>();

	entitySig.reset(bitPosition);

	entityHandler.SetSignature(entity, entitySig);

	systemManager.EntitySignatureChanged(entity, entitySig);
}

template <typename T>
T* EntityComponentSystem::GetComponent(Entity entity)
{
	if (entityHandler.DoesEntityExist(entity))
	{
		return componentManager.GetComponent<T>(entity);
	}
}

template <typename T>
std::shared_ptr<SparseSet<T>> EntityComponentSystem::GetComponentSet()
{
	return componentManager.GetComponentSet<T>();
} 

template <typename SystemName, typename... ComponentNames>
std::shared_ptr<SystemName> EntityComponentSystem::RegisterSystem(std::shared_ptr<SystemName> system)
{
	Signature systemSignature;

	// set signature by folding over each of the given components
	(systemSignature.set(componentManager.GetBitPosition<ComponentNames>()), ...);

	return systemManager.RegisterSystem<SystemName>(systemSignature, system);
}