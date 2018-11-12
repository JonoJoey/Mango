#pragma once

#include "../../common.h"

#include "entity_3d.h"


namespace Mango
{
	class Light3D : public Entity3D
	{
	public:
		Light3D() = default;
		Light3D(glm::vec3 position, glm::vec3 color = { 1.f, 1.f, 1.f }) : m_color(color) { SetPosition(position); }

		void SetColor(const glm::vec3& color) { m_color = color; }
		glm::vec3 GetColor() const { return m_color; }

	private:
		glm::vec3 m_color = { 1.f, 1.f, 1.f };
	};
} // namespace Mango