#pragma once

#include <vector>
#include <iostream>
#include "Entity.h"

constexpr int EMPTY_INDEX = -1;

template <typename T>
struct Value
{
	T component;
	Entity entity; //TODO: swap for actual "entity" when it exists
};

class ComponentSet
{
public:
	virtual ~ComponentSet() = default;
	//virtual void EntityDestroyed(Entity entity) = 0;
};


template <typename T>
class SparseSet : public ComponentSet
{
public:
	SparseSet(int maxEntities);
	void Insert(Entity entity, T component);
	void Delete(Entity entity);
	T* DataAt(Entity entity);

	// stores indices into dense
	std::vector<int> sparse;

	// stores actual data
	std::vector<Value<T>> dense;

	//void printSet(); // for debugging
private:
	int maxEntities;
};

template <typename T>
SparseSet<T>::SparseSet(int maxEntities)
{
	this->maxEntities = maxEntities;
	sparse.resize(maxEntities);
	std::fill(sparse.begin(), sparse.end(), EMPTY_INDEX);
}

template <typename T>
void SparseSet<T>::Insert(Entity entity, T component)
{
	Value value = { component,entity };
	if (value.entity >= maxEntities)
	{
		std::cout << "Maximum Sparse Set Size Reached\n";
		return;
	}

	const auto pos = dense.size();
	dense.push_back(value);
	sparse[value.entity] = pos;
	
}

template <typename T>
void SparseSet<T>::Delete(Entity entity)
{

	// update dense by swapping removed entity and last entity
	const auto backEntity = dense.back();
	
	// replace deleted entry with last entity in dense
	dense[sparse[entity]] = backEntity;

	// update last entry's index
	sparse[backEntity.entity] = sparse[entity];

	// remove back entry and deleted entity's index
	dense.pop_back();
	sparse[entity] = EMPTY_INDEX;
}

template <typename T>
T* SparseSet<T>::DataAt(Entity entity)
{
	if (entity >= maxEntities || sparse[entity] == EMPTY_INDEX)
	{
		return nullptr;
	}

	//std::cout << dense[sparse[entity]].component << "\n"; // debug
	return &dense[sparse[entity]].component;
}


// for debugging
/*
template <typename T>
void SparseSet<T>::printSet()
{
	std::cout << "SPARSE: [";
	for (auto i : sparse)
	{
		std::cout << i << ", ";
	}
	std::cout << "]\n";

	std::cout << "DENSE: [";
	for (auto j : dense)
	{
		std::cout << "(" << j.component << ", " << j.entity << "), ";
	}
	std::cout << "]\n\n\n";
}
*/