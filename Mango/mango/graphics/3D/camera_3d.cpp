#include "camera_3d.h"


namespace Mango
{
	glm::mat4 Camera3D::GetViewMatrix() const
	{
		// view matrix
		return glm::lookAt(m_position, m_position + Maths::AngleVector(m_viewangle), { 0.f, 1.f, 0.f });
	}
} // namespace Mango