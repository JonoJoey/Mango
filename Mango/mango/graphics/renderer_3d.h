#pragma once

#include "../common.h"


namespace Mango
{
	class MangoCore;

	class Renderer3D : private Utility
	{
	public:
		Renderer3D() = default;
		~Renderer3D() { Release(); }

		bool Setup(MangoCore* mango);
		void Release() override;

		void Start();
		void End();

	private:
		MangoCore* m_mango = nullptr;
	};
} // namespace Mango