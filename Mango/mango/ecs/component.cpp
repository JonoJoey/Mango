#include "component.h"


namespace Mango::ECS
{
	COMPONENT_ID NextID()
	{
		static COMPONENT_ID id = 0;
		return id++;
	}

} // namespace Mango::ECS