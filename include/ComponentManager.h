#pragma once

#include "SparseSet.h"
#include "Entity.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

class ComponentManager
{
public:
	ComponentManager(int maxEntities);

	template <typename T>
	void RegisterComponent(); // add a new component to the array

	template <typename T>
	void AddComponent(Entity entity, T component); // adds a component to the array for an entity

	template<typename T>
	void RemoveComponent(Entity entity); // removes a component from an array for an entity

	template <typename T>
	T* GetComponent(Entity entity); // get a component from an array for an entity

	void EntityDeleted(Entity entity); // remove all components from all arrays for an entity

	template <typename T>
	std::shared_ptr<SparseSet<T>> GetComponentSet(); // get the SparseSet of a component

private:
	std::unordered_map<std::string, std::shared_ptr<ComponentSet>> componentArrays;
	int maxEntities;
};
 
inline ComponentManager::ComponentManager(int maxEntities)
{
	this->maxEntities = maxEntities;
}

template<typename T>
void ComponentManager::RegisterComponent()
{
	std::string componentName = typeid(T).name();

	if (componentArrays.find(componentName) != componentArrays.end()) // pretty sure this is done in the get part
	{
		std::cout << "Component Already Registered\n";
		return;
	}

	componentArrays.insert({ componentName, std::make_shared<SparseSet<T>>(maxEntities) });
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
	std::shared_ptr<SparseSet<T>> componentArray = GetComponentSet<T>(); // should this be a reference?
	componentArray->Insert(entity, component);
}


template<typename T>
void ComponentManager::RemoveComponent(Entity entity)
{
	std::shared_ptr<SparseSet<T>> componentArray = GetComponentSet<T>(); // should this be a reference?
	//TODO: check for null
	componentArray->Delete(entity);
}

template<typename T>
T* ComponentManager::GetComponent(Entity entity)
{
	std::shared_ptr<SparseSet<T>> componentArray = GetComponentSet<T>(); // should this be a reference?
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