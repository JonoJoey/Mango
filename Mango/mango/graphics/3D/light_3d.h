#pragma once

#include "../../common.h"

#include "entity_3d.h"


namespace Mango
{
	class Light3D : public Entity3D
	{
	public:
		Light3D() = default;
		Light3D(glm::vec3 position) { SetPosition(position); }

		void SetAmbientColor(const glm::vec3& color) { m_ambient_color = color; }
		glm::vec3 GetAmbientColor() const { return m_ambient_color; }

		void SetDiffuseColor(const glm::vec3& color) { m_diffuse_color = color; }
		glm::vec3 GetDiffuseColor() const { return m_diffuse_color; }

		void SetSpecularColor(const glm::vec3& color) { m_specular_color = color; }
		glm::vec3 GetSpecularColor() const { return m_specular_color; }

	private:
		glm::vec3 m_ambient_color = { 1.f, 1.f, 1.f };
		glm::vec3 m_diffuse_color = { 1.f, 1.f, 1.f };
		glm::vec3 m_specular_color = { 1.f, 1.f, 1.f };
	};
} // namespace Mango