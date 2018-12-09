#include "renderer_2d.h"

#include "../../mango.h"


namespace
{
	static const auto flat_shader_fs_src = R"(
#version 330 core

layout(location = 0) out vec4 out_color;

uniform vec3 u_flat_color;

void main()
{
    vec4 color = vec4(u_flat_color, 1.0);
    out_color = color;
}
)";
	static const auto flat_shader_vs_src = R"(
#version 330 core

layout(location = 0) in vec3 in_position;

uniform mat4 u_projection_matrix;

void main()
{
    gl_Position = u_projection_matrix * vec4(in_position, 1.0);
}
)";

	static const auto texture_shader_fs_src = R"(
#version 330 core

layout(location = 0) out vec4 out_color;

in vec2 _tex_coord;

uniform sampler2D u_texture;

void main()
{
    vec4 color = texture(u_texture, _tex_coord);
	if (color.a < 0.01)
		discard;

    out_color = color;
}
)";
	static const auto texture_shader_vs_src = R"(
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_tex_coord;

out vec2 _tex_coord;

uniform mat4 u_projection_matrix;

void main()
{
    gl_Position = u_projection_matrix * vec4(in_position, 1.0);
	_tex_coord = in_tex_coord;
}
)";
}


namespace Mango
{
	bool Renderer2D::Setup(MangoCore* mango)
	{
		m_mango = mango;

		if (!m_text_renderer.Setup())
			return false;

		// matrices
		const auto window_size = mango->GetWindowSize();
		m_proj_matrix = glm::ortho(0.f, float(window_size[0]), float(window_size[1]), 0.f);

		// flat shader
		m_flat_shader.Setup(flat_shader_vs_src, flat_shader_fs_src);
		m_flat_shader.Bind();
		m_flat_shader.SetUniformMat4("u_projection_matrix", m_proj_matrix);
		Shader::Unbind();

		// texture shader
		m_texture_shader.Setup(texture_shader_vs_src, texture_shader_fs_src);
		m_texture_shader.Bind();
		m_texture_shader.SetUniformMat4("u_projection_matrix", m_proj_matrix);
		Shader::Unbind();

		// quad model
		{
			static const unsigned int indices[] =
			{
				0, 1, 2,
				2, 3, 0
			};
			static const float tex_coords[] =
			{
				0.f, 1.f,
				0.f, 0.f,
				1.f, 0.f,
				1.f, 1.f
			};

			m_quad_model.Setup(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
			m_quad_model.GetVAO().Bind();

			// positions
			auto vbo = &m_quad_model.AddVBO();
			vbo->Setup(sizeof(int) * 4 * 2, nullptr);
			vbo->Bind();
			VertexArray::EnableAttribute(0, 2, GL_INT, false, 0, 0);

			// tex coords
			vbo = &m_quad_model.AddVBO();
			vbo->Setup(sizeof(float) * 4 * 2, tex_coords);
			vbo->Bind();
			VertexArray::EnableAttribute(1, 2, GL_FLOAT, false, 0, 0);

			VertexArray::Unbind();
		}

		is_init = true;
		return true;
	}
	void Renderer2D::Release()
	{
		if (!is_init)
			return;

		is_init = false;
		m_quad_model.Release();

		m_flat_shader.Release();
		m_texture_shader.Release();

		m_text_renderer.Release();
	}
	void Renderer2D::Resize(int width, int height)
	{
		m_proj_matrix = glm::ortho(0.f, float(width), float(height), 0.f);

		// flat shader
		m_flat_shader.Bind();
		m_flat_shader.SetUniformMat4("u_projection_matrix", m_proj_matrix);
		Shader::Unbind();

		// texture shader
		m_texture_shader.Bind();
		m_texture_shader.SetUniformMat4("u_projection_matrix", m_proj_matrix);
		Shader::Unbind();
	}

	void Renderer2D::Start()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// disable depth test
		glDisable(GL_DEPTH_TEST);

		// disable culling
		glDisable(GL_CULL_FACE);
	}
	void Renderer2D::End()
	{

	}

	FONT_HANDLE Renderer2D::NewFont(std::string name, std::string file_path, int font_height)
	{
		return m_text_renderer.NewFont(name, file_path, font_height);
	}

	void Renderer2D::RenderQuad(glm::ivec2 pos1, glm::ivec2 pos2, glm::vec3 color)
	{
		const int vertices[] =
		{
			pos1[0], pos1[1], // top left
			pos1[0], pos2[1], // bottom left
			pos2[0], pos2[1], // bottom right
			pos2[0], pos1[1] // top right
		};

		m_quad_model.GetVAO().Bind();
		m_flat_shader.Bind();
		m_flat_shader.SetUniformF3("u_flat_color", color);

		m_quad_model.GetVBOs()[0].Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		VertexBuffer::Unbind();

		glDrawElements(m_quad_model.GetMode(), m_quad_model.GetIBO().GetCount(), m_quad_model.GetIBO().GetType(), nullptr);

		Shader::Unbind();
		VertexArray::Unbind();
	}
	void Renderer2D::RenderTexturedQuad(glm::ivec2 pos1, glm::ivec2 pos2)
	{
		const int vertices[] =
		{
			pos1[0], pos1[1], // top left
			pos1[0], pos2[1], // bottom left
			pos2[0], pos2[1], // bottom right
			pos2[0], pos1[1] // top right
		};

		m_quad_model.GetVAO().Bind();
		m_texture_shader.Bind();

		m_quad_model.GetVBOs()[0].Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		VertexBuffer::Unbind();

		glDrawElements(m_quad_model.GetMode(), m_quad_model.GetIBO().GetCount(), m_quad_model.GetIBO().GetType(), nullptr);

		Shader::Unbind();
		VertexArray::Unbind();
	}
	bool Renderer2D::RenderText(FONT_HANDLE font, const glm::ivec2& position, const glm::vec4& color, const char* format, ...)
	{
		char buffer[1024];
		va_list arg_list;
		va_start(arg_list, format);
		vsnprintf(buffer, 1024, format, arg_list);
		va_end(arg_list);

		return m_text_renderer.RenderText(font, m_proj_matrix, position, color, buffer);
	}
} // namespace Mango