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

		void RenderTriangle(const TrianglePos2D& pos, const TriangleColor2D& color);
		void RenderTriangle(glm::i16vec2 vert_1, glm::i16vec2 vert_2, glm::i16vec2 vert_3, glm::u8vec4 color_1, glm::u8vec4 color_2, glm::u8vec4 color_3)
		{
			RenderTriangle({ vert_1, vert_2, vert_3 }, { color_1, color_2, color_3 });
		}
		void RenderTriangle(glm::i16vec2 vert_1, glm::i16vec2 vert_2, glm::i16vec2 vert_3, glm::u8vec4 color)
		{
			RenderTriangle({ vert_1, vert_2, vert_3 }, { color, color, color });
		}

		void RenderTexturedTriangle(const TrianglePos2D& pos, const TriangleTexCoord2D& tex_coord, const Texture& texture);
		void RenderTexturedTriangle(glm::i16vec2 vert_1, glm::i16vec2 vert_2, glm::i16vec2 vert_3, glm::fvec2 tex_coord_1, glm::fvec2 tex_coord_2, glm::fvec2 tex_coord_3, const Texture& texture)
		{
			RenderTexturedTriangle({ vert_1, vert_2, vert_3 }, { tex_coord_1, tex_coord_2, tex_coord_3 }, texture);
		}

		void SetProjMatrix(const glm::mat4& mat) { m_proj_matrix = mat; }
		const glm::mat4& GetProjMatrix() const { return m_proj_matrix; }

		void SetViewMatrix(const glm::mat4& mat) { m_view_matrix = mat; }
		const glm::mat4& GetViewMatrix() const { return m_view_matrix; }

	private:
		void RenderQueue();

	private:
		MangoCore* m_mango = nullptr;
		Shader m_basic_shader,
			m_texture_shader;
		glm::mat4 m_proj_matrix,
			m_view_matrix;
		
		struct // m_triangle_queue
		{
			static constexpr size_t MAX_TRIANGLES = 50;
			size_t m_num_triangles = 0;

			// arrays of size MAX_TRIANGLES
			TrianglePos2D* m_positions = nullptr;
			TriangleColor2D* m_colors = nullptr;

			VertexArray m_vao;
			VertexBuffer m_position_buffer,
				m_color_buffer;
		} m_triangle_queue;

		struct // m_textured_triangles
		{
			VertexArray m_vao;
			VertexBuffer m_position_buffer,
				m_tex_coord_buffer;
		} m_textured_triangles;
	};
} // namespace Mango