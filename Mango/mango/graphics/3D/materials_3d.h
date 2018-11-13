#pragma once

#include "../wrappers/texture.h"


namespace Mango
{
	struct Material3D
	{
		Texture* diffuse_map = nullptr;
		Texture* specular_map = nullptr;
		float shine_damper;
	};
} // namespace Mango