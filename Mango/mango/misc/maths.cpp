#include "maths.h"

#include "../common.h"


namespace Mango::Maths
{
	glm::vec3 AngleVector(glm::vec3 angle)
	{
		angle.x = DEG2RAD(angle.x);
		angle.y = DEG2RAD(angle.y);
		angle.z = DEG2RAD(angle.z);

		return glm::normalize(glm::vec3(
			cos(angle[1]) * sin(angle[0]),
			sin(angle[1]),
			-cos(angle[1]) * cos(angle[0])
		));
	}

	glm::mat4 CreateViewMatrix(const glm::vec3& position, const glm::vec3& angle)
	{
		return glm::lookAt(position, position + Maths::AngleVector(angle), { 0.f, 1.f, 0.f });
	}
	glm::mat4 CreateModelMatrix(const glm::vec3& position, const glm::vec3& rotation, float scale)
	{
		glm::mat4 mat = glm::translate(glm::mat4(1.f), position);
		mat = glm::rotate(mat, DEG2RAD(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		mat = glm::rotate(mat, DEG2RAD(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		mat = glm::rotate(mat, DEG2RAD(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		mat = glm::scale(mat, glm::vec3(scale, scale, scale));
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