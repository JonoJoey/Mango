#pragma once

#include "../common.h"

#include "wrappers/shader.h"
#include "wrappers/vertex_array.h"
#include "wrappers/buffer_object.h"


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
		
		struct
		{
			static constexpr size_t MAX_TRIANGLES = 50;
			size_t m_num_triangles = 0;

			// arrays of size MAX_TRIANGLES * 3
			TrianglePos2D* m_positions = nullptr;
			TriangleColor2D* m_colors = nullptr;

			VertexArray m_vao;
			VertexBuffer m_position_buffer,
				m_color_buffer;
		} m_triangle_queue;
	};
} // namespace Mango