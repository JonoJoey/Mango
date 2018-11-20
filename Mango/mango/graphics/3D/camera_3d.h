#pragma once

#include "../../common.h"


namespace Mango
{
	class Camera3D
	{
	public:
		Camera3D(const glm::vec3& position = { 0.f, 0.f, 0.f }, const glm::vec3& viewangle = { 0.f, 0.f, 0.f })
			: m_position(position), m_viewangle(viewangle) { ComputeMatrix(); }

		const glm::mat4& GetViewMatrix() const;

		void Move(const glm::vec3& offset);
		void SetPosition(const glm::vec3& position);
		glm::vec3 GetPosition();

		void SetViewangle(const glm::vec3& viewangle /* radians */);
		glm::vec3 GetViewangle();

	private:
		void ComputeMatrix();

	private:
		glm::vec3 m_position,
			m_viewangle;
		glm::mat4 m_view_matrix;
	};
} // namespace Mango