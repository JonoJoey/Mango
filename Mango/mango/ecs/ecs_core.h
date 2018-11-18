#pragma once

#include <unordered_map>

#include "../common.h"

#include "component.h"
#include "system.h"


namespace Mango::ECS
{
	typedef uint32_t ENTITY_HANDLE;
	static constexpr auto INVALID_ENTITY_HANDLE = ENTITY_HANDLE(0);

	typedef void(*ECS_SYSTEM_CALLBACK)(std::deque<ENTITY_HANDLE>, std::unordered_map<COMPONENT_ID, std::deque<BaseComponent*>>);

	class ECSCore
	{
	public:
		ECSCore() = default;
		~ECSCore() { Release(); }

		void Process();
		void Release();

		ENTITY_HANDLE CreateEntity();
		void RemoveEntity(ENTITY_HANDLE handle);

		template <typename T, typename... pack>
		bool AttachComponent(ENTITY_HANDLE handle, pack... args)
		{
			if (auto res = m_entities.find(handle); res != m_entities.end())
			{
				for (auto comp : res->second->components)
				{
					// component already exists
					if (comp->id == T::ID)
						return false;
				}
			}
			else // entity doesn't exist
				return false;

			BaseComponent* comp = new T(args...);

			comp->id = T::ID;
			comp->name = T::NAME;
			comp->size = T::SIZE;

			AttachComponent(handle, comp);
			return true;
		}

		void AttachSystem(std::vector<COMPONENT_ID> filter, ECS_SYSTEM_CALLBACK callback);

	private:
		struct Entity
		{
			ENTITY_HANDLE handle;
			std::deque<std::shared_ptr<BaseComponent>> components;
		};
		struct System
		{
			std::vector<COMPONENT_ID> filter;
			ECS_SYSTEM_CALLBACK callback;
		};

		ENTITY_HANDLE GenerateNewEntityHandle();
		void AttachComponent(ENTITY_HANDLE handle, BaseComponent* comp);

	private:
		std::unordered_map<ENTITY_HANDLE, std::shared_ptr<Entity>> m_entities;
		std::vector<System> m_systems;
	};
} // namespace Mango::ECS