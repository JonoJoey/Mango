#include "camera_3d.h"


namespace Mango
{
	glm::mat4 Camera3D::GetViewMatrix() const
	{
		return Maths::CreateViewMatrix(m_position, m_viewangle);
	}

	void Camera3D::Move(const glm::vec3& offset) 
	{ 
		SetPosition(m_position + offset);
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
		m_viewangle = Maths::NormalizeAngle(viewangle);
	}
	glm::vec3 Camera3D::GetViewangle() 
	{ 
		return m_viewangle; 
	}
} // namespace Mango