#pragma once

#include "../../common.h"


namespace Mango
{
	class Camera3D
	{
	public:
		Camera3D() = default;
		Camera3D(const glm::vec3& position, const glm::vec3& viewangle = { 0.f, 0.f, 0.f }) { m_position = position; m_viewangle = viewangle; }

		glm::mat4 GetViewMatrix() const;

		void SetPosition(const glm::vec3& position) { m_position = position; }
		glm::vec3 GetPosition() { return m_position; }

		void SetViewangle(const glm::vec3& viewangle) { m_viewangle = viewangle; }
		glm::vec3 GetViewangle() { return m_viewangle; }

	private:
		glm::vec3 m_position = { 0.f, 0.f, 0.f };
		glm::vec3 m_viewangle = { 0.f, 0.f, 0.f };
	};
} // namespace Mango