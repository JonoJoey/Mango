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

	void Renderer3D::Start()
	{

	}
	void Renderer3D::End()
	{

	}
} // namespace Mango