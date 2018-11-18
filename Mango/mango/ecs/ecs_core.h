#pragma once

#include <unordered_map>

#include "../common.h"

#include "component.h"
#include "system.h"


namespace Mango::ECS
{
	class ECSCore
	{
	public:
		ECSCore() = default;
		~ECSCore() { Release(); }

		void OnTick();
		void OnFrame();

		void Release();

		ENTITY_HANDLE CreateEntity();
		void RemoveEntity(ENTITY_HANDLE handle);

		template <typename T, typename... pack>
		bool AttachComponent(ENTITY_HANDLE handle, pack... args)
		{
			// entity doesn't exist
			if (m_entities.find(handle) == m_entities.end())
				return false;

			if (auto res = m_components.find(T::ID); res != m_components.end())
			{
				if (res->second.find(handle) != res->second.end())
					return false;
			}

			BaseComponent* comp = new T(args...);
			comp->id = T::ID;
			comp->name = T::NAME;
			comp->entity = handle;
			AttachComponent(comp);
			return true;
		}

		void AttachSystem(ECSSystem* system);

	private:
		struct Entity
		{
			ENTITY_HANDLE handle;

		};
		
		ENTITY_HANDLE GenerateNewEntityHandle();
		void AttachComponent(BaseComponent* comp);

	private:
		std::unordered_map<ENTITY_HANDLE, Entity> m_entities;
		std::unordered_map<COMPONENT_ID, std::unordered_map<ENTITY_HANDLE, std::shared_ptr<BaseComponent>>> m_components;
		std::vector<ECSSystem*> m_systems;
	};
} // namespace Mango::ECS