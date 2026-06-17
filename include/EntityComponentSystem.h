#pragma once

#include "Entity.h"
#include "EntityHandler.h"
#include "ComponentManager.h"
#include "SparseSet.h"

class EntityComponentSystem
{
public:
	EntityComponentSystem(int maxEntities);
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

private:
	EntityHandler entityHandler;
	ComponentManager componentManager;
};

inline EntityComponentSystem::EntityComponentSystem(int maxEntities) : entityHandler(maxEntities), componentManager(maxEntities)
{
}

inline Entity EntityComponentSystem::CreateEntity()
{
	return entityHandler.CreateEntity();
}

inline void EntityComponentSystem::DestroyEntity(Entity entity)
{
	entityHandler.DeleteEntity(entity); //TODO change delete to destroy
	componentManager.EntityDeleted(entity);
}

template <typename T>
void EntityComponentSystem::RegisterComponent()
{
	componentManager.RegisterComponent<T>(); // do i need <T> here?
}

template <typename T>
void EntityComponentSystem::AddComponent(Entity entity, T component)
{
	componentManager.AddComponent<T>(entity, component);
}

template <typename T>
void EntityComponentSystem::RemoveComponent(Entity entity)
{
	componentManager.RemoveComponent<T>(entity);
}

template <typename T>
T* EntityComponentSystem::GetComponent(Entity entity)
{
	return componentManager.GetComponent<T>(entity);
}

template <typename T>
std::shared_ptr<SparseSet<T>>  EntityComponentSystem::GetComponentSet()
{
	return componentManager.GetComponentSet<T>();
} // maybe store sets of components and just get them from the manager?