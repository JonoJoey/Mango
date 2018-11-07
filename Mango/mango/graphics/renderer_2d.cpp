#include "renderer_2d.h"


namespace Mango
{
	bool Renderer2D::Setup()
	{
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