#pragma once

#include "ECSComponents.h"
#include <bitset>
#include <vector>

typedef uint32_t EntityIndex;
typedef uint32_t EntityVersion;
typedef uint64_t EntityID;

const int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> ComponentsMask;
const int MAX_ENTITIES = 100;

struct ComponentPool;

struct Entity
{
	EntityID id;
	ComponentsMask componentsMask;
};

class EntityRegistry
{
private:
	static std::vector<Entity> m_entities;
	static std::vector<ComponentPool*> m_componentPools;

	static std::vector<EntityIndex> m_freedEntities;
	static int s_componentCounter;

public:
	template <class T>
	static int GetComponentID()
	{
		static int s_componentId = s_componentCounter++;
		return s_componentId;
	}

	static EntityID NewEntity();
	static void DestroyEntity(EntityID entityID);

	static EntityID GetEntityID(EntityIndex index);
	static ComponentsMask GetEntityComponentsMask(EntityIndex index);
	static size_t GetNumOfEntities();

	static void Release();

	template <typename Component>
	static Component* AssignComponent(EntityID entityID)
	{
		if (m_entities[GetEntityIndex(entityID)].id != entityID)
			return nullptr;

		int componentId = GetComponentID<Component>();
		m_entities[GetEntityIndex(entityID)].componentsMask.set(componentId);

		if (m_componentPools.size() <= componentId)
		{
			m_componentPools.resize(componentId + 1, nullptr);
		}
		if (m_componentPools[componentId] == nullptr)
		{
			m_componentPools[componentId] = new ComponentPool(sizeof(Component));
		}

		Component* pComponent = new (m_componentPools[componentId]->Get(GetEntityIndex(entityID))) Component();
		return pComponent;
	}

	template <typename Component>
	static Component* GetComponent(EntityID entityID)
	{
		if (m_entities[GetEntityIndex(entityID)].id != entityID)
			return nullptr;

		int componentId = GetComponentID<Component>();
		if (!m_entities[GetEntityIndex(entityID)].componentsMask.test(componentId))
			return nullptr;

		Component* pComponent = static_cast<Component*>(m_componentPools[componentId]->Get(GetEntityIndex(entityID)));
		return pComponent;
	}

	template<typename Component>
	static void RemoveComponent(EntityID entityID)
	{
		if (m_entities[GetEntityIndex(entityID)].id != entityID)
			return;

		int componentId = GetComponentID<Component>();
		m_entities[GetEntityIndex(entityID)].componentsMask.reset(componentId);
	}


	static inline EntityID CreateEntityID(EntityIndex index, EntityVersion version)
	{
		return ((EntityID)index << 32) | ((EntityID)version);
	}
	static inline EntityIndex GetEntityIndex(EntityID entityID)
	{
		return entityID >> 32;
	}
	static inline EntityVersion GetEntityVersion(EntityID entityID)
	{
		return (EntityVersion)entityID;
	}
	static inline bool IsEntityValid(EntityID entityID)
	{
		return (entityID >> 32) != EntityIndex(-1);
	}

private:
	EntityRegistry() {};
};
template<typename... ComponentTypes>
struct RegistryView
{
	RegistryView()
	{
		if (sizeof...(ComponentTypes) == 0) {
			all = true;
		}
		else {
			int componentIds[] = { 0, EntityRegistry::GetComponentID<ComponentTypes>() ... };
			for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
				componentsMask.set(componentIds[i]);
		}
	}

	struct Iterator
	{
		Iterator(EntityIndex index, ComponentsMask mask, bool all)
			: index(index), mask(mask), all(all) {}

		EntityID operator*() const
		{
			return EntityRegistry::GetEntityID(index);
		}

		bool operator==(const Iterator& other) const
		{
			return index == other.index || index == EntityRegistry::GetNumOfEntities();
		}

		bool operator!=(const Iterator& other) const
		{
			return index != other.index && index != EntityRegistry::GetNumOfEntities();
		}

		bool ValidIndex()
		{
			return EntityRegistry::IsEntityValid(EntityRegistry::GetEntityID(index)) &&
				(all || mask == (mask & EntityRegistry::GetEntityComponentsMask(index)));
		}

		Iterator& operator++()
		{
			do {
				index++;
			} while (index < EntityRegistry::GetNumOfEntities() && !ValidIndex());
			return *this;
		}

		EntityIndex index;
		ComponentsMask mask;
		bool all{ false };
	};
	const Iterator begin() const
	{
		int firstIndex = 0;
		while (firstIndex < EntityRegistry::GetNumOfEntities() &&
			(componentsMask != (componentsMask & EntityRegistry::GetEntityComponentsMask(firstIndex))
				|| !EntityRegistry::IsEntityValid(EntityRegistry::GetEntityID(firstIndex))))
		{
			firstIndex++;
		}
		return Iterator(firstIndex, componentsMask, all);
	}

	const Iterator end() const
	{
		return Iterator(EntityIndex(EntityRegistry::GetNumOfEntities()), componentsMask, all);
	}
	ComponentsMask componentsMask;
	bool all{ false };
};

struct ComponentPool
{
	ComponentPool(size_t elementSize)
	{
		this->elementSize = elementSize;
		pData = new char[elementSize * MAX_ENTITIES];
	}

	~ComponentPool()
	{
		delete[] pData;
	}

	inline void* Get(EntityIndex index)
	{
		return pData + index * elementSize;
	}

private:
	char* pData{ nullptr };
	size_t elementSize{ 0 };
};