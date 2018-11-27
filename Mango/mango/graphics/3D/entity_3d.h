#pragma once

#include "../../common.h"


namespace Mango
{
	class Entity3D
	{
	public:
		Entity3D() = default;
		Entity3D(const glm::vec3& position, const glm::vec3& rotation = { 0.f, 0.f, 0.f }, float scale = 1.f)
			: m_position(position), m_rotation(rotation), m_scale(scale) {};

		glm::mat4 GetModelMatrix() const { return Maths::CreateModelMatrix(m_position, m_rotation, m_scale); }

		void SetPosition(const glm::vec3& position) { m_position = position; }
		glm::vec3 GetPosition() const { return m_position; }

		void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; }
		glm::vec3 GetRotation() const { return m_rotation; }

		void SetScale(float scale) { m_scale = scale; }
		float GetScale() const { return m_scale; }

	private:
		glm::vec3 m_position = { 0.f, 0.f, 0.f };
		glm::vec3 m_rotation = { 0.f, 0.f, 0.f };
		float m_scale = 1.f;
	};
} // namespace Mango