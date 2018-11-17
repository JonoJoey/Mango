#include "entity.h"

#include <unordered_map>
#include <memory>


namespace
{
	using namespace Mango::ECS;

	struct Entity
	{
		std::deque<std::shared_ptr<BaseComponent>> components;
	};

	std::unordered_map<ENTITY_HANDLE, std::shared_ptr<Entity>> entities;

	ENTITY_HANDLE GetNextEntityHandle()
	{
		static ENTITY_HANDLE last_handle = INVALID_ENTITY_HANDLE + 1;

		while (last_handle == INVALID_ENTITY_HANDLE || entities.find(last_handle) != entities.end())
			last_handle++;

		return last_handle;
	}
}


namespace Mango::ECS
{
	ENTITY_HANDLE CreateEntity()
	{
		ENTITY_HANDLE handle = GetNextEntityHandle();

		auto entity = new Entity;
		entities[handle] = std::shared_ptr<Entity>(entity);

		return handle;
	}
	void RemoveEntity(ENTITY_HANDLE entity)
	{
		entities.erase(entity);
	}

	void AddComponent(ENTITY_HANDLE entity, BaseComponent* comp)
	{
		entities[entity]->components.emplace_back(comp);
	}

	void Test(ENTITY_HANDLE entity, ECS_CALLBACK callback)
	{
		printf("0x%X\n", entity);

		callback(entities[entity]->components);
	}
} // namespace Mango::ECS