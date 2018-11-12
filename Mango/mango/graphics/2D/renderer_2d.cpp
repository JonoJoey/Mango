#include "renderer_2d.h"

#include "../../mango.h"


namespace Mango
{
	bool Renderer2D::Setup(MangoCore* mango)
	{
		m_mango = mango;

		// matrices
		const auto window_size = mango->GetWindowSize();
		m_proj_matrix = glm::ortho(0.f, float(window_size[0]), float(window_size[1]), 0.f);
		m_view_matrix = glm::mat4(1.f);

		return true;
	}
	void Renderer2D::Release()
	{
		if (!IsUtilRegistered())
			return;
		UnregisterUtil();
	}

	void Renderer2D::Start()
	{

	}
	void Renderer2D::End()
	{

	}
} // namespace Mango