#pragma once

#include <glm.hpp>


namespace Mango::Maths
{
	glm::vec3 AngleVector(const glm::vec3& angle);

	glm::mat4 CreateModelMatrix(const glm::vec3& position, const glm::vec3& rotation, float scale = 1.f);
	glm::mat4 CreateProjectionMatrix(float fov /* degrees */, float aspect_ratio, float znear, float zfar);
} // namespace Mango::Maths