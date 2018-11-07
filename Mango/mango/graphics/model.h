#pragma once

#include "../common.h"


namespace Mango
{
	class Model : private Utility
	{
	public:
		Model() = default;
		~Model() { Release(); }

		bool Setup();
		void Release() override;

	private:
	};
} // namespace Mango