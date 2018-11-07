#pragma once

#include "../common.h"

#include "wrappers/shader.h"
#include "wrappers/vertex_array.h"
#include "wrappers/buffer_object.h"
#include "wrappers/texture.h"


namespace Mango
{
	class MangoCore;

	struct TrianglePos2D
	{
		glm::i16vec2 m_vertex_1;
		glm::i16vec2 m_vertex_2;
		glm::i16vec2 m_vertex_3;
	};
	struct TriangleColor2D
	{
		glm::u8vec4 m_color_1;
		glm::u8vec4 m_color_2;
		glm::u8vec4 m_color_3;
	};
	struct TriangleTexCoord2D
	{
		glm::fvec2 m_tex_coord_1;
		glm::fvec2 m_tex_coord_2;
		glm::fvec2 m_tex_coord_3;
	};

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
		Shader m_basic_shader,
			m_texture_shader;
		glm::mat4 m_proj_matrix,
			m_view_matrix;
	};
} // namespace Mango