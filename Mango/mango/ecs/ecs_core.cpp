#include "ecs_core.h"


namespace Mango::ECS
{
	void ECSCore::Process()
	{
		for (auto system : m_systems)
		{
			std::deque<ENTITY_HANDLE> entities;
			std::unordered_map<COMPONENT_ID, std::deque<BaseComponent*>> components;

			for (auto entity : m_entities)
			{
				std::deque<BaseComponent*> comps;
				for (auto comp : entity.second->components)
				{
					if (std::find(system.filter.begin(), system.filter.end(), comp->id) != system.filter.end())
						comps.push_back(comp->GetPtr());
				}
				if (comps.size() != system.filter.size())
					continue;

				entities.push_back(entity.first);
				for (auto comp : comps)
					components[comp->id].push_back(comp);
			}

			system.callback(entities, components);
		}
	}
	void ECSCore::Release()
	{
		m_entities.clear();
	}

	ENTITY_HANDLE ECSCore::CreateEntity()
	{
		const auto handle = GenerateNewEntityHandle();

		auto entity = new Entity;
		entity->handle = handle;
		m_entities[handle] = std::shared_ptr<Entity>(entity);

		return handle;
	}
	void ECSCore::RemoveEntity(ENTITY_HANDLE handle)
	{
		m_entities.erase(handle);
	}
	ENTITY_HANDLE ECSCore::GenerateNewEntityHandle()
	{
		ENTITY_HANDLE handle = INVALID_ENTITY_HANDLE + 1;

		if (m_entities.empty())
			return handle;

		while (handle == INVALID_ENTITY_HANDLE || m_entities.find(handle) != m_entities.end())
			handle++;

		return handle;
	}

	void ECSCore::AttachComponent(ENTITY_HANDLE handle, BaseComponent* comp)
	{
		m_entities.at(handle)->components.emplace_back(comp);
	}
	void ECSCore::AttachSystem(std::vector<COMPONENT_ID> filter, ECS_SYSTEM_CALLBACK callback)
	{
		if (!filter.empty())
			m_systems.push_back({ filter, callback });
	}
} // namespace Mango::ECS