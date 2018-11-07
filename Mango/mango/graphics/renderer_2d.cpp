#include "renderer_2d.h"

#include "../mango.h"


namespace Mango
{
	bool Renderer2D::Setup(MangoCore* mango)
	{
		m_mango = mango;

		// matrices
		const auto window_size = mango->GetWindowSize();
		m_proj_matrix = glm::ortho(0.f, float(window_size[0]), float(window_size[1]), 0.f);
		m_view_matrix = glm::mat4(1.f);

		// shaders
		ASSERT(m_basic_shader.Setup(Shader::ReadFile("res/shaders/basic_vs.glsl"), Shader::ReadFile("res/shaders/basic_fs.glsl")));
		ASSERT(m_texture_shader.Setup(Shader::ReadFile("res/shaders/textured_vs.glsl"), Shader::ReadFile("res/shaders/textured_fs.glsl")));

		// m_triangle_queue
		m_triangle_queue.m_num_triangles = 0;
		m_triangle_queue.m_vao.Setup();
		m_triangle_queue.m_vao.Bind();

		m_basic_shader.Bind();

		m_triangle_queue.m_positions = new TrianglePos2D[m_triangle_queue.MAX_TRIANGLES * 3];
		m_triangle_queue.m_position_buffer.Setup(m_triangle_queue.MAX_TRIANGLES * 3, sizeof(TrianglePos2D), nullptr, GL_DYNAMIC_DRAW);
		m_triangle_queue.m_position_buffer.Bind();
		VertexArray::EnableAttribute(0, 2, GL_SHORT, GL_FALSE, 0, 0);
		VertexBuffer::Unbind();

		m_triangle_queue.m_colors = new TriangleColor2D[m_triangle_queue.MAX_TRIANGLES * 3];
		m_triangle_queue.m_color_buffer.Setup(m_triangle_queue.MAX_TRIANGLES * 3, sizeof(TriangleColor2D), nullptr, GL_DYNAMIC_DRAW);
		m_triangle_queue.m_color_buffer.Bind();
		VertexArray::EnableAttribute(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
		VertexBuffer::Unbind();
		VertexArray::Unbind();

		return true;
	}
	void Renderer2D::Release()
	{
		if (!IsUtilRegistered())
			return;
		UnregisterUtil();

		// shaders
		m_basic_shader.Release();
		m_texture_shader.Release();

		// m_triangle_queue
		m_triangle_queue.m_vao.Release();
		m_triangle_queue.m_position_buffer.Release();
		m_triangle_queue.m_color_buffer.Release();
		delete[] m_triangle_queue.m_positions;
		delete[] m_triangle_queue.m_colors;
	}

	void Renderer2D::Start()
	{
		ASSERT(m_triangle_queue.m_num_triangles == 0);
	}
	void Renderer2D::End()
	{
		RenderQueue();
	}

	void Renderer2D::RenderQueue()
	{
		// nothing to render...
		if (m_triangle_queue.m_num_triangles <= 0)
			return;

		m_triangle_queue.m_vao.Bind();
		m_basic_shader.Bind();

		Shader::SetUniformMat4(m_basic_shader.GetUniformLoc("u_ProjectionMatrix"), m_proj_matrix);
		Shader::SetUniformMat4(m_basic_shader.GetUniformLoc("u_ViewMatrix"), m_view_matrix);

		// update buffers
		m_triangle_queue.m_position_buffer.Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_triangle_queue.m_num_triangles * 3 * sizeof(TrianglePos2D), m_triangle_queue.m_positions);
		m_triangle_queue.m_color_buffer.Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_triangle_queue.m_num_triangles * 3 * sizeof(TriangleColor2D), m_triangle_queue.m_colors);

		// draw
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, m_triangle_queue.m_num_triangles * 3);
		glEnable(GL_DEPTH_TEST);

		VertexBuffer::Unbind();
		Shader::Unbind();
		VertexArray::Unbind();

		m_triangle_queue.m_num_triangles = 0;
	}

	void Renderer2D::RenderTriangle(const TrianglePos2D& pos, const TriangleColor2D& color)
	{
		// queue is full
		if (m_triangle_queue.m_num_triangles == m_triangle_queue.MAX_TRIANGLES)
			RenderQueue();

		m_triangle_queue.m_positions[m_triangle_queue.m_num_triangles] = pos;
		m_triangle_queue.m_colors[m_triangle_queue.m_num_triangles] = color;

		m_triangle_queue.m_num_triangles++;
	}
} // namespace Mango