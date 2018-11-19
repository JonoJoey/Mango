#include "renderer_3d.h"


namespace Mango
{
	bool Renderer3D::Setup(MangoCore* mango)
	{
		m_mango = mango;

		return true;
	}
	void Renderer3D::Release()
	{
		if (!IsUtilRegistered())
			return;
		UnregisterUtil();


	}

	void Renderer3D::Resize(int width, int height)
	{

	}

	void Renderer3D::Start()
	{
		glEnable(GL_BLEND);
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