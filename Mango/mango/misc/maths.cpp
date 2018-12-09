#include "maths.h"

#include "../common.h"


namespace Mango::Maths
{
	glm::dvec3 AngleVector(glm::dvec3 angle)
	{
		angle.x = glm::radians(angle.x);
		angle.y = glm::radians(angle.y);
		angle.z = glm::radians(angle.z);

		return glm::normalize(glm::dvec3(
			cos(angle[1]) * sin(angle[0]),
			sin(angle[1]),
			-cos(angle[1]) * cos(angle[0])
		));
	}

	glm::mat4 CreateViewMatrix(const glm::dvec3& position, const glm::dvec3& angle)
	{
		return glm::lookAt(position, position + Maths::AngleVector(angle), { 0.0, 1.0, 0.0 });
	}
	glm::mat4 CreateModelMatrix(const glm::dvec3& position, const glm::dvec3& rotation, double scale)
	{
		glm::dmat4 mat = glm::translate(glm::dmat4(1.0), position);
		mat = glm::rotate(mat, glm::radians(rotation.x), glm::dvec3(1.0, 0.0, 0.0));
		mat = glm::rotate(mat, glm::radians(rotation.y), glm::dvec3(0.0, 1.0, 0.0));
		mat = glm::rotate(mat, glm::radians(rotation.z), glm::dvec3(0.0, 0.0, 1.0));
		mat = glm::scale(mat, glm::dvec3(scale, scale, scale));
		return mat;
	}
	glm::mat4 CreateProjectionMatrix(float fov, float aspect_ratio, float znear, float zfar)
	{
		return glm::perspective(glm::radians(fov), aspect_ratio, znear, zfar);
	}
	glm::mat3 CreateNormalMatrix(const glm::mat4& model_matrix)
	{
		return glm::mat3(glm::transpose(glm::inverse(model_matrix)));
	}
} // namespace Mango::Maths