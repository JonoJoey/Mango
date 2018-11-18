#include "ecs_core.h"


namespace Mango::ECS
{
	void ECSCore::OnTick()
	{
		for (auto system : m_systems)
		{
			std::unordered_map<COMPONENT_ID, const std::unordered_map<ENTITY_HANDLE, std::shared_ptr<BaseComponent>>*> comps;

			const auto& filter = system->GetFilter();
			if (filter.empty())
			{
				for (const auto comp : m_components)
					comps[comp.first] = &m_components[comp.first];
			}
			else
			{
				for (const auto comp_id : filter)
					comps[comp_id] = &m_components[comp_id];
			}

			system->OnTick(comps);
		}
	}
	void ECSCore::OnFrame()
	{
		for (auto system : m_systems)
		{
			std::unordered_map<COMPONENT_ID, const std::unordered_map<ENTITY_HANDLE, std::shared_ptr<BaseComponent>>*> comps;

			const auto& filter = system->GetFilter();
			if (filter.empty())
			{
				for (const auto comp : m_components)
					comps[comp.first] = &m_components[comp.first];
			}
			else
			{
				for (const auto comp_id : filter)
					comps[comp_id] = &m_components[comp_id];
			}

			system->OnFrame(comps);
		}
	}

	void ECSCore::Release()
	{
		m_entities.clear();
	}

	ENTITY_HANDLE ECSCore::CreateEntity()
	{
		const auto handle = GenerateNewEntityHandle();

		Entity entity;
		entity.handle = handle;
		m_entities[handle] = entity;

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

	void ECSCore::AttachComponent(BaseComponent* comp)
	{
		m_components[comp->id][comp->entity] = std::shared_ptr<BaseComponent>(comp);
	}
	void ECSCore::AttachSystem(ECSSystem* system)
	{
		system->OnAttach();
		m_systems.push_back(system);
	}
} // namespace Mango::ECS