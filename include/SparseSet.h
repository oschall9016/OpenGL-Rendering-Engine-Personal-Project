#pragma once

#include "Entity.h"

#include <vector>
#include <iostream>

constexpr int EMPTY_INDEX = -1;

template <typename T>
struct Value
{
	T component;
	Entity entity;
};


class UniversalSparseSet
{
public:
	virtual ~UniversalSparseSet() = default;
	virtual void Delete(Entity entity) = 0;
};



template <typename T>
class SparseSet : public UniversalSparseSet
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
	
	if (sparse[value.entity] != EMPTY_INDEX)
	{
		std::cout << "Entity:" << entity << " Already Inserted\n";
		return;
	}

	const auto pos = dense.size();
	dense.push_back(value);
	sparse[value.entity] = pos;
	
}

template <typename T>
void SparseSet<T>::Delete(Entity entity)
{

	if (sparse[entity] == EMPTY_INDEX)
	{
		std::cout << "Entity:" << entity << " Does Not Exist\n";
		return;
	}

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

	return &dense[sparse[entity]].component;
}