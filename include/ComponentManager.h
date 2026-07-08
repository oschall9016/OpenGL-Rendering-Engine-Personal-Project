#pragma once

#include "SparseSet.h"
#include "Entity.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include <queue>
#include <typeindex>

class ComponentManager
{
public:
	ComponentManager();

	template <typename ComponentType>
	void RegisterComponent(); // add a new component to the array

	template <typename ComponentType>
	const int GetBitPosition(); // returns the bit position for a given component

	template <typename ComponentType>
	void AddComponent(Entity entity, ComponentType component); // adds a component to the array for an entity

	template<typename ComponentType>
	void RemoveComponent(Entity entity); // removes a component from an array for an entity

	template <typename ComponentType>
	ComponentType* GetComponent(Entity entity); // get a component from an array for an entity

	void EntityDeleted(Entity entity); // remove all components from all arrays for an entity

	template <typename ComponentType>
	std::shared_ptr<SparseSet<ComponentType>> GetComponentSet(); // get the specific SparseSet of a USS stored in componentArrays

private:
	std::unordered_map<std::type_index, std::shared_ptr<UniversalSparseSet>> componentTypeUSSMap;
	std::unordered_map<std::type_index, int> componentTypeBitMap;
	std::queue<int> componentBitsQueue;
};
 
inline ComponentManager::ComponentManager()
{
	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		componentBitsQueue.push(i);
	}
}

template<typename ComponentType>
void ComponentManager::RegisterComponent()
{
	if (componentTypeUSSMap.find(typeid(ComponentType)) != componentTypeUSSMap.end())
	{
		std::cout << "Component Already Registered\n";
		return;
	}

	if (componentBitsQueue.empty())
	{
		std::cout << "Cannot Register Component: " << typeid(ComponentType).name() << " Max Number of Components Reached\n";
		return;
	}

	int bit = componentBitsQueue.front();
	componentBitsQueue.pop();

	componentTypeBitMap.insert({ typeid(ComponentType), bit});
	componentTypeUSSMap.insert({ typeid(ComponentType), std::make_shared<SparseSet<ComponentType>>(MAX_ENTITIES) });
}

template <typename ComponentType>
const int ComponentManager::GetBitPosition()
{
	return componentTypeBitMap.at(typeid(ComponentType));
}

template<typename ComponentType>
void ComponentManager::AddComponent(Entity entity, ComponentType component)
{
	if (componentTypeUSSMap.find(typeid(ComponentType)) == componentTypeUSSMap.end())
	{
		std::cout << "Component Is Not Registered\n";
		return;
	}
	std::shared_ptr<SparseSet<ComponentType>> componentSet = GetComponentSet<ComponentType>();
	componentSet->Insert(entity, component);
}


template<typename ComponentType>
void ComponentManager::RemoveComponent(Entity entity)
{
	std::shared_ptr<SparseSet<ComponentType>> componentSet = GetComponentSet<ComponentType>();

	componentSet->Delete(entity);
}

template<typename ComponentType>
ComponentType* ComponentManager::GetComponent(Entity entity)
{
	std::shared_ptr<SparseSet<ComponentType>> componentSet = GetComponentSet<ComponentType>();
	return componentSet->DataAt(entity);
}

inline void ComponentManager::EntityDeleted(Entity entity)
{
	for (const auto& componentTypeUSSPair : componentTypeUSSMap)
	{
		const auto& componentSet = componentTypeUSSPair.second;
		componentSet->Delete(entity);
	}
}

template<typename ComponentType>
std::shared_ptr<SparseSet<ComponentType>> ComponentManager::GetComponentSet()
{
	auto componentTypeUSSPair = componentTypeUSSMap.find(typeid(ComponentType));

	if (componentTypeUSSPair == componentTypeUSSMap.end())
	{
		std::cout << "Cannot Get Component Set, " << typeid(ComponentType).name() << ", Component Is Not Registered\n";
		return nullptr;
	}

	return std::static_pointer_cast<SparseSet<ComponentType>>(componentTypeUSSPair->second);
}