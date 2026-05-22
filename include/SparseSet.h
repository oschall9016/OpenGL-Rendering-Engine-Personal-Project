#pragma once

#include <vector>
#include <iostream>

constexpr int EMPTY_INDEX = -1;

template <typename T>
struct Value
{
	T component;
	int entity; //TODO: swap for actual "entity" when it exists
};

template <typename T>
class SparseSet
{
public:
	SparseSet(int maxSize);
	void Insert(Value<T> value);
	void Delete(int index);
	T* DataAt(int index);

	void printSet(); // for debugging
private:
	// stores indices into dense
	std::vector<int> sparse;

	// stores actual data
	std::vector<Value<T>> dense;

	int maxSize;
};

template <typename T>
SparseSet<T>::SparseSet(int maxSize)
{
	this->maxSize = maxSize;
	sparse.resize(maxSize);
	std::fill(sparse.begin(), sparse.end(), -1);
}

template <typename T>
void SparseSet<T>::Insert(Value<T> value)
{
	if (value.entity >= maxSize)
	{
		std::cout << "Maximum Sparse Set Size Reached\n";
		return;
	}

	const auto pos = dense.size();
	dense.push_back(value);
	sparse[value.entity] = pos;
	
}

template <typename T>
void SparseSet<T>::Delete(int entity)
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
T* SparseSet<T>::DataAt(int entity)
{
	if (entity >= maxSize || sparse[entity] == EMPTY_INDEX)
	{
		return nullptr;
	}

	std::cout << dense[sparse[entity]].component << "\n"; // debug
	return &dense[sparse[entity]].component;
}


// for debugging
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