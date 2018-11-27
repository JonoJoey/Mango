#include "camera_3d.h"


namespace Mango
{
	const glm::mat4& Camera3D::GetViewMatrix() const
	{
		return m_view_matrix;
	}
	void Camera3D::ComputeMatrix()
	{
		m_view_matrix = Maths::CreateViewMatrix(m_position, m_viewangle);
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
		m_viewangle.x = Maths::NormalizeFloat(viewangle.x, -180.f, 180.f);
		m_viewangle.y = Maths::ClampFloat(viewangle.y, -89.f, 89.f);
		m_viewangle.z = Maths::NormalizeFloat(viewangle.z, -180.f, 180.f);
		
		ComputeMatrix();
	}
	glm::vec3 Camera3D::GetViewangle() 
	{ 
		return m_viewangle; 
	}
} // namespace Mango