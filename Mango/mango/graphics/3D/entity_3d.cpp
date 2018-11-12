#include "entity_3d.h"


namespace Mango
{
	glm::mat4 Entity3D::GetModelMatrix() const
	{
		return Maths::CreateModelMatrix(m_position, m_rotation, m_scale);
	}
} // namespace Mango