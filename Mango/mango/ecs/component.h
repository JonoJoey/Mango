#pragma once

#include <string>

#define COMPONENT_INFO(component, name) const char* const Mango::ECS::Component<component>::NAME = name; const size_t Mango::ECS::Component<component>::SIZE = sizeof(component)


namespace Mango::ECS
{
	typedef uint32_t COMPONENT_ID;

	COMPONENT_ID NextID();

	struct BaseComponent
	{
		COMPONENT_ID id;
		const char* name;
		size_t size;

		BaseComponent* GetPtr() { return this; }
	};

	template <typename T>
	struct Component : public BaseComponent
	{
		static const COMPONENT_ID ID;
		static const char* const NAME;
		static const size_t SIZE;
	};

	template <typename T>
	const COMPONENT_ID Component<T>::ID = NextID();
} // namespace Mango::ECS