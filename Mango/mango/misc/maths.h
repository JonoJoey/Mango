#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm.hpp>

#define M_PI_F float(M_PI)

#define RAD2DEG(x) float(x * (180.f / M_PI_F))
#define DEG2RAD(x) float(x * M_PI_F / 180.0);


namespace Mango::Maths
{
	glm::vec3 AngleVector(const glm::vec3& angle);

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
	
	inline glm::vec3 NormalizeRadianAngle(glm::vec3 angle)
	{
		angle.x = NormalizeFloat(angle.x, -M_PI_F, M_PI_F);
		angle.y = NormalizeFloat(angle.y, -M_PI_F, M_PI_F);
		angle.z = NormalizeFloat(angle.z, -M_PI_F, M_PI_F);
		return angle;
	}
} // namespace Mango::Maths