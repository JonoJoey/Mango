#include "camera_3d.h"


namespace Mango
{
	const glm::mat4& Camera3D::GetViewMatrix() const
	{
		return m_view_matrix;
	}
	void Camera3D::ComputeMatrix()
	{
		m_view_matrix = glm::lookAt(m_position, m_position + Maths::AngleVector(m_viewangle), { 0.f, 1.f, 0.f });
	}

	void Camera3D::Move(const glm::vec3& offset) 
	{ 
		SetPosition(m_position + offset);
	}
	void Camera3D::SetPosition(const glm::vec3& position)
	{ 
		m_position = position;
		ComputeMatrix();
	}
	glm::vec3 Camera3D::GetPosition() 
	{ 
		return m_position; 
	}

	void Camera3D::SetViewangle(const glm::vec3& viewangle) 
	{ 
		m_viewangle = Maths::NormalizeRadianAngle(viewangle);
		ComputeMatrix();
	}
	glm::vec3 Camera3D::GetViewangle() 
	{ 
		return m_viewangle; 
	}
} // namespace Mango