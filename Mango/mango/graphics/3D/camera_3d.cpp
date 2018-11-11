#include "camera_3d.h"


namespace Mango
{
	glm::mat4 Camera3D::GetViewMatrix() const
	{
		// view matrix
		return glm::lookAt(m_position, m_position + Maths::AngleVector(m_viewangle), { 0.f, 1.f, 0.f });
	}

	void Camera3D::Move(const glm::vec3& offset) 
	{ 
		m_position += offset; 
	}
	void Camera3D::SetPosition(const glm::vec3& position)
	{ 
		m_position = position; 
	}
	glm::vec3 Camera3D::GetPosition() 
	{ 
		return m_position; 
	}

	void Camera3D::SetViewangle(const glm::vec3& viewangle) 
	{ 
		m_viewangle = Maths::NormalizeRadianAngle(viewangle);
	}
	glm::vec3 Camera3D::GetViewangle() 
	{ 
		return m_viewangle; 
	}
} // namespace Mango