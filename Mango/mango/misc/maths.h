#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm.hpp>

#define M_PI_F float(M_PI)
#define M_PI_2F float(M_PI_2)

#define RAD2DEG(x) float(x * (180.f / M_PI_F))
#define DEG2RAD(x) float(x * M_PI_F / 180.f)


namespace Mango::Maths
{
	glm::vec3 AngleVector(glm::vec3 angle);

	glm::mat4 CreateViewMatrix(const glm::vec3& position, const glm::vec3& angle);
	glm::mat4 CreateModelMatrix(const glm::vec3& position, const glm::vec3& rotation, float scale = 1.f);
	glm::mat4 CreateProjectionMatrix(float fov /* degrees */, float aspect_ratio, float znear, float zfar);
	glm::mat3 CreateNormalMatrix(const glm::mat4& model_matrix);

	// TODO: fastER!
	// normalize from [min, max]
	inline float NormalizeFloat(float num, float min, float max)
	{
		const float diff = max - min;
		while (num < min)
			num += diff;
		while (num > max)
			num -= diff;
		return num;
	}
	inline float ClampFloat(float num, float min, float max)
	{
		if (num < min)
			return min;
		if (num > max)
			return max;
		return num;
	}
} // namespace Mango::Maths