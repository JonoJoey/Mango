#pragma once

#include "../common.h"


namespace Mango
{
	class MangoCore;

	class Renderer3D : private Utility
	{
	public:
		Renderer3D() = default;
		~Renderer3D() { Release(); }

		bool Setup(MangoCore* mango);
		void Release() override;

		void Start();
		void End();

		void SetProjMatrix(const glm::mat4& mat) { m_proj_matrix = mat; }
		const glm::mat4& GetProjMatrix() const { return m_proj_matrix; }

		void SetViewMatrix(const glm::mat4& mat) { m_view_matrix = mat; }
		const glm::mat4& GetViewMatrix() const { return m_view_matrix; }

	private:
		MangoCore* m_mango = nullptr;
		glm::mat4 m_proj_matrix,
			m_view_matrix;
	};
} // namespace Mango