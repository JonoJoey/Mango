#pragma once

#include <string>
#include <deque>

#define NEW_COMPONENT(component) struct component : public Mango::ECS::Component<component>
#define COMPONENT_INFO(component, name) const char* Mango::ECS::Component<component>::NAME = name; const size_t Mango::ECS::Component<component>::SIZE = sizeof(component)


namespace Mango::ECS
{
	typedef void* ENTITY_HANDLE;

	int NextID();

	struct BaseComponent
	{
		ENTITY_HANDLE entity;
	};

	template <typename T>
	struct Component : public BaseComponent
	{
		static const int ID;
		static const char* NAME;
		static const size_t SIZE;
	};

	template <typename T>
	const int Component<T>::ID = NextID();

	template <typename T>
	BaseComponent* CreateComponent()
	{
		return &T();
	}
} // namespace Mango::ECS