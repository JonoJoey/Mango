#include "renderer_3d.h"

#include "../../mango.h"


namespace Mango
{
	bool Renderer3D::Setup(MangoCore* mango)
	{
		m_mango = mango;

		const auto window_size = mango->GetWindowSize();
		SetProjMatrix(Mango::Maths::CreateProjectionMatrix(60.f, float(window_size[0]) / float(window_size[1]), 1.f, -1.f));

		is_init = true;
		return true;
	}
	void Renderer3D::Release()
	{
		if (!is_init)
			return;

		is_init = false;
	}

	void Renderer3D::Resize(int width, int height)
	{
		SetProjMatrix(Mango::Maths::CreateProjectionMatrix(60.f, float(width) / float(height), 1.f, -1.f));
	}

	void Renderer3D::Start()
	{
		//glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// enable depth test
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// enable culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	void Renderer3D::End()
	{

	}
} // namespace Mango