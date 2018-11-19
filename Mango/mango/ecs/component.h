#pragma once

#include <string>

#define NEW_COMPONENT(component) struct component : public Mango::ECS::Component<component>
#define COMPONENT_INFO(component, name) const char* const Mango::ECS::Component<component>::NAME = name;


namespace Mango::ECS
{
	typedef uint32_t COMPONENT_ID;
	typedef uint32_t ENTITY_HANDLE;
	static constexpr auto INVALID_ENTITY_HANDLE = ENTITY_HANDLE(0);

	COMPONENT_ID NextID();

	struct BaseComponent
	{
		COMPONENT_ID id;
		ENTITY_HANDLE entity;
		const char* name;

		BaseComponent* GetPtr() { return this; }
	};

	template <typename T>
	struct Component : public BaseComponent
	{
		static const COMPONENT_ID ID;
		static const char* const NAME;
	};

	template <typename T>
	const COMPONENT_ID Component<T>::ID = NextID();
} // namespace Mango::ECS