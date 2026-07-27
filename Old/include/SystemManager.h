#pragma once

#include "Entity.h"
#include "System.h"

#include <unordered_map>
#include <string>
#include <memory>

class SystemManager
{
public:
	SystemManager();

	template <typename SystemName>
	std::shared_ptr<SystemName> RegisterSystem(Signature signature, std::shared_ptr<SystemName> system); // add a system and signature to the corresponding maps to be updated

	void EntityDestroyed(Entity entity); // remove entity from all systems
	void EntitySignatureChanged(Entity entity, Signature entitySignature); // check if entity needs to be added to or removed from all systems

private:
	std::unordered_map<std::string, std::shared_ptr<System>> typeSystemMap;
	std::unordered_map<std::string, Signature> typeSignatureMap;
};

inline SystemManager::SystemManager(){}

template <typename SystemName>
std::shared_ptr<SystemName> SystemManager::RegisterSystem(Signature signature, std::shared_ptr<SystemName> system)
{
	std::string systemName = typeid(SystemName).name();

	typeSystemMap.insert({ systemName, system });
	typeSignatureMap.insert({ systemName, signature });

	return system;
}

inline void SystemManager::EntityDestroyed(Entity entity)
{
	// loop over each system and remove entity
	for (const auto& typeSystemPair : typeSystemMap)
	{
		const auto& system = typeSystemPair.second;
		system->entities.erase(entity);
	}
}

inline void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
	// loop over all systems and test if entity needs added or deleted
	for (const auto& typeSystemPair : typeSystemMap)
	{
		const auto& type = typeSystemPair.first;
		const auto& system = typeSystemPair.second;

		const Signature& systemSignature = typeSignatureMap.at(type);

		if ( (entitySignature & systemSignature) == systemSignature)
		{
			system->entities.insert(entity); // since entities is a set it ignores duplicates
		}
		else
		{
			system->entities.erase(entity);
		}
		
	}
}
