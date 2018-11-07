#include "maths.h"

#include "../common.h"


namespace Mango::Maths
{
	glm::vec3 AngleVector(const glm::vec3& angle)
	{
		return glm::normalize(glm::vec3(
			cos(angle[1]) * sin(angle[0]),
			sin(angle[1]),
			-cos(angle[1]) * cos(angle[0])
		));
	}
} // namespace Mango::Maths