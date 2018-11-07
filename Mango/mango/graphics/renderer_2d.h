#pragma once

#include "../common.h"


namespace Mango
{
	class Renderer2D : private Utility
	{
	public:
		Renderer2D() = default;
		~Renderer2D() { Release(); }

		bool Setup();
		void Release() override;

		void Start();
		void End();

	private:
	};
} // namespace Mango