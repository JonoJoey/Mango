#pragma once

#include "component.h"

#include <deque>


namespace Mango::ECS
{
	ENTITY_HANDLE CreateEntity();
	void RemoveEntity(ENTITY_HANDLE entity);

	void AddComponent(ENTITY_HANDLE entity, BaseComponent* comp);

	template <typename T, class... pack>
	void AddComponent(ENTITY_HANDLE entity, pack... args)
	{
		auto comp = new T(args...);
		comp->name = T::NAME;
		comp->size = T::SIZE;
		comp->id = T::ID;
		AddComponent(entity, comp);
	}

	typedef void(*ECS_CALLBACK)(std::deque<std::shared_ptr<BaseComponent>>);
	void Test(ENTITY_HANDLE entity, ECS_CALLBACK callback);
} // namespace Mango::ECS