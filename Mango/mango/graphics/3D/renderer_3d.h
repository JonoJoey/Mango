#pragma once

#include "../../common.h"

#include "camera_3d.h"
#include "light_3d.h"
#include "entity_3d.h"
#include "materials_3d.h"


namespace Mango
{
	class MangoCore;

	class Renderer3D : private Utility
	{
	public:
		Renderer3D() = default;
		~Renderer3D() { Release(); UnregisterUtil(); }

		bool Setup(MangoCore* mango);
		void Release() override;
		void Resize(int width, int height);

		void Start();
		void End();

		void SetProjMatrix(const glm::mat4& mat) { m_proj_matrix = mat; }
		const glm::mat4& GetProjMatrix() const { return m_proj_matrix; }

	private:
		bool is_init = false;
		MangoCore* m_mango = nullptr;
		glm::mat4 m_proj_matrix;
	};
} // namespace Mango