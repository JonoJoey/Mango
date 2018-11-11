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

	glm::mat4 CreateModelMatrix(const glm::vec3& position, const glm::vec3& rotation, float scale)
	{
		glm::mat4 mat = glm::translate(glm::mat4(1.f), position);
		mat = glm::rotate(mat, rotation.x, glm::vec3(1.f, 0.f, 0.f));
		mat = glm::rotate(mat, rotation.y, glm::vec3(0.f, 1.f, 0.f));
		mat = glm::rotate(mat, rotation.z, glm::vec3(0.f, 0.f, 1.f));
		mat = glm::scale(mat, glm::vec3(scale, scale, scale));
		return mat;
	}
	glm::mat4 CreateProjectionMatrix(float fov, float aspect_ratio, float znear, float zfar)
	{
		return glm::perspective(glm::radians(fov), aspect_ratio, znear, zfar);
	}
} // namespace Mango::Maths