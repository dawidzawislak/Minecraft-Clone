#include "ECS.h"

std::vector<Entity> EntityRegistry::m_entities;
std::vector<ComponentPool*> EntityRegistry::m_componentPools;
std::vector<EntityIndex> EntityRegistry::m_freedEntities;
int EntityRegistry::s_componentCounter = 0;

EntityID EntityRegistry::NewEntity()
{
	if (!m_freedEntities.empty()) {
		EntityIndex newIndex = m_freedEntities.back();
		m_freedEntities.pop_back();
		EntityID newID = CreateEntityID(newIndex, GetEntityVersion(m_entities[newIndex].id));
		m_entities[newIndex].id = newID;
		return m_entities[newIndex].id;
	}
	m_entities.push_back({ CreateEntityID(EntityIndex(m_entities.size()), 0), ComponentsMask() });
	return m_entities.back().id;
}

void EntityRegistry::DestroyEntity(EntityID entityID)
{
	EntityID newID = CreateEntityID(EntityIndex(-1), GetEntityVersion(entityID) + 1);
	m_entities[GetEntityIndex(entityID)].id = newID;
	m_entities[GetEntityIndex(entityID)].componentsMask.reset();
	m_freedEntities.push_back(GetEntityIndex(entityID));
}

EntityID EntityRegistry::GetEntityID(EntityIndex index)
{
	return m_entities[index].id;
}

ComponentsMask EntityRegistry::GetEntityComponentsMask(EntityIndex index)
{
	return m_entities[index].componentsMask;
}

size_t EntityRegistry::GetNumOfEntities()
{
	return m_entities.size();
}

void EntityRegistry::Release()
{
	for (ComponentPool* pool : m_componentPools)
		delete pool;
}