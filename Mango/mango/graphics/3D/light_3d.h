#pragma once

#include "../../common.h"


namespace Mango
{
	class Light3D
	{
	public:
		Light3D() = default;
		Light3D(glm::vec3 position, glm::vec3 color = { 1.f, 1.f, 1.f }) : m_position(position), m_color(color) {}

	private:
		glm::vec3 m_position = { 0.f, 0.f, 0.f };
		glm::vec3 m_color = { 1.f, 1.f, 1.f };
	};
} // namespace Mango