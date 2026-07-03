#pragma once

#include "SparseSet.h"
#include "Entity.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include <queue>

class ComponentManager
{
public:
	ComponentManager();

	template <typename T>
	void RegisterComponent(); // add a new component to the array

	template <typename t>
	const int GetBitPosition(); // returns the bit position for a given component

	template <typename T>
	void AddComponent(Entity entity, T component); // adds a component to the array for an entity

	template<typename T>
	void RemoveComponent(Entity entity); // removes a component from an array for an entity

	template <typename T>
	T* GetComponent(Entity entity); // get a component from an array for an entity

	void EntityDeleted(Entity entity); // remove all components from all arrays for an entity

	template <typename T>
	std::shared_ptr<SparseSet<T>> GetComponentSet(); // get the SparseSet of a component set

private:
	std::unordered_map<std::string, std::shared_ptr<ComponentSet>> componentArrays; // TODO: change naming convention
	std::unordered_map<std::string, int> componentTypeBitMap;
	std::queue<int> componentBits;
};
 
inline ComponentManager::ComponentManager()
{
	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		componentBits.push(i);
	}
}

template<typename T>
void ComponentManager::RegisterComponent()
{
	std::string componentName = typeid(T).name();

	if (componentArrays.find(componentName) != componentArrays.end())
	{
		std::cout << "Component Already Registered\n";
		return;
	}

	if (componentTypeBitMap.empty())
	{
		std::cout << "Cannot Register Component: " << componentName << " Max Number of Components Reached\n";
		return;
	}

	int bit = componentBits.front();
	componentBits.pop();

	componentTypeBitMap.insert({componentName, bit});
	componentArrays.insert({ componentName, std::make_shared<SparseSet<T>>(MAX_ENTITIES) });
}

template <typename t>
const int ComponentManager::GetBitPosition()
{
	std::string componentName = typeid(T).name();

	return componentTypeBitMap.at(componentName);
}

template<typename T>
void ComponentManager::AddComponent(Entity entity, T component)
{
	std::string componentName = typeid(T).name();

	//TODO: check if entity exists?
	if (componentArrays.find(componentName) == componentArrays.end())
	{
		std::cout << "Component Is Not Registered\n";
		return;
	}
	std::shared_ptr<SparseSet<T>> componentArray = GetComponentSet<T>(); 
	componentArray->Insert(entity, component);
}


template<typename T>
void ComponentManager::RemoveComponent(Entity entity)
{
	std::shared_ptr<SparseSet<T>> componentArray = GetComponentSet<T>();
	//TODO: check for null
	componentArray->Delete(entity);
}

template<typename T>
T* ComponentManager::GetComponent(Entity entity)
{
	std::shared_ptr<SparseSet<T>> componentArray = GetComponentSet<T>(); 
	return componentArray->DataAt(entity);
}

inline void ComponentManager::EntityDeleted(Entity entity)
{
	for (const auto& pair : componentArrays)
	{
		const auto& componentSet = pair.second;
		componentSet->Delete(entity);
	}
}

template<typename T>
std::shared_ptr<SparseSet<T>> ComponentManager::GetComponentSet()
{
	std::string componentName = typeid(T).name();
	auto component = componentArrays.find(componentName);

	if (component == componentArrays.end())
	{
		std::cout << "Cannot Get Component Set, Component Is Not Registered\n";
		return nullptr;
	}

	return std::static_pointer_cast<SparseSet<T>>(component->second);
}