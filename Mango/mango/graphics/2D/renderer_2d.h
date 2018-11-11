#pragma once

#include "../../common.h"

#include "../wrappers/shader.h"
#include "../wrappers/vertex_array.h"
#include "../wrappers/buffer_object.h"
#include "../wrappers/texture.h"


namespace Mango
{
	class MangoCore;

	class Renderer2D : private Utility
	{
	public:
		Renderer2D() = default;
		~Renderer2D() { Release(); }

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