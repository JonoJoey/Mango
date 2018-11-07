#pragma once

#include "../../common.h"


namespace Mango
{
	class Texture : private Utility
	{
	public:
		Texture() = default;
		~Texture() { Release(); }

		bool Setup();
		void Release() override;

	private:
		unsigned int m_texture = 0xFFFFFFFF;
	};
} // namespace Mango