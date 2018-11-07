#include "texture.h"


namespace Mango
{
	bool Texture::Setup()
	{
		return true;
	}
	void Texture::Release()
	{
		if (!IsUtilRegistered())
			return;
		UnregisterUtil();

		if (m_texture == 0xFFFFFFFF)
			return;


	}
} // namespace Mango