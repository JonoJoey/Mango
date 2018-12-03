#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <glm.hpp>

#include <algorithm>


namespace Mango::Maths
{
	glm::vec3 AngleVector(glm::vec3 angle);

	glm::mat4 CreateViewMatrix(const glm::vec3& position, const glm::vec3& angle);
	glm::mat4 CreateModelMatrix(const glm::vec3& position, const glm::vec3& rotation = { 0.f, 0.f, 0.f }, float scale = 1.f);
	glm::mat4 CreateProjectionMatrix(float fov /* degrees */, float aspect_ratio, float znear, float zfar);
	glm::mat3 CreateNormalMatrix(const glm::mat4& model_matrix);

	// TODO: fastER!
	// normalize from [min, max]
	template <typename T>
	inline T NormalizeNum(T num, T min, T max)
	{
		const T diff = max - min;
		while (num < min)
			num += diff;
		while (num > max)
			num -= diff;
		return num;
	}
	template <typename T>
	inline T ApproachZero(T x, T speed)
	{
		if (x > T(0))
			x = glm::max<T>(T(0), x - speed);
		else if (x < T(0))
			x = glm::min<T>(T(0), x + speed);

		return x;
	};
	inline glm::vec3 NormalizeAngle(glm::vec3 angle)
	{
		angle.x = NormalizeNum(angle.x, -180.f, 180.f);
		angle.y = std::clamp(angle.y, -89.f, 89.f);
		angle.z = NormalizeNum(angle.z, -180.f, 180.f);
		return angle;
	}
} // namespace Mango::Maths