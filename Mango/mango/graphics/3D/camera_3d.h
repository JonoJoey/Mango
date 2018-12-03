#pragma once

#include "../../common.h"


namespace Mango
{
	class Camera3D
	{
	public:
		Camera3D(const glm::dvec3& position = { 0.0, 0.0, 0.0 }, const glm::vec3& viewangle = { 0.f, 0.f, 0.f })
			: m_position(position), m_viewangle(viewangle) {  }

		void Move(const glm::dvec3& offset);
		void SetPosition(const glm::dvec3& position);
		glm::dvec3 GetPosition();

		void SetViewangle(const glm::vec3& viewangle);
		glm::vec3 GetViewangle();

	private:
		glm::dvec3 m_position;
		glm::vec3 m_viewangle;
	};
} // namespace Mango