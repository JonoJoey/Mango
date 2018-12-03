#include "camera_3d.h"


namespace Mango
{
	void Camera3D::Move(const glm::dvec3& offset) 
	{ 
		SetPosition(m_position + offset);
	}
	void Camera3D::SetPosition(const glm::dvec3& position)
	{ 
		m_position = position;
	}
	glm::dvec3 Camera3D::GetPosition() 
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