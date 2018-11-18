#pragma once

#include <unordered_map>
#include <vector>

#include "component.h"


namespace Mango::ECS
{
	class ECSSystem
	{
	public:
		ECSSystem() = default;
		virtual ~ECSSystem() {};

		virtual void OnAttach() {};
		virtual void OnFrame(std::unordered_map<COMPONENT_ID, const std::unordered_map<ENTITY_HANDLE, std::shared_ptr<BaseComponent>>*>) {}
		virtual void OnTick(std::unordered_map<COMPONENT_ID, const std::unordered_map<ENTITY_HANDLE, std::shared_ptr<BaseComponent>>*>) {}

		void SetFilter(const std::vector<COMPONENT_ID>& filter) { m_filter = filter; }
		const std::vector<COMPONENT_ID>& GetFilter() { return m_filter; }

	private:
		std::vector<COMPONENT_ID> m_filter;
	};
} // namespace Mango::ECS