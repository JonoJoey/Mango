#pragma once

#include <string>

#define NEW_COMPONENT(component) struct component : public Mango::ECS::Component<component>
#define COMPONENT_INFO(component, name) const char* Mango::ECS::Component<component>::NAME = name; const size_t Mango::ECS::Component<component>::SIZE = sizeof(component)

#define ENTITY_HANDLE uint32_t
#define INVALID_ENTITY_HANDLE ENTITY_HANDLE(0)


namespace Mango::ECS
{
	int NextID();

	struct BaseComponent
	{
		ENTITY_HANDLE entity;
		const char* name;
		size_t size;
		int id;

		void* GetPtr() { return this; }
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
} // namespace Mango::ECS