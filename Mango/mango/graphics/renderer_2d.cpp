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
	}

	void Renderer2D::Start()
	{

	}
	void Renderer2D::End()
	{

	}
} // namespace Mango