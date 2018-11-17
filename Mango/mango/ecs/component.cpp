#include "component.h"


namespace Mango::ECS
{
	int NextID()
	{
		// start at 1, not 0
		static int id = 1;
		return id++;
	}
} // namespace Mango::ECS