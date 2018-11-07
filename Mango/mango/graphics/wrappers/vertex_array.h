#pragma once

#include "../../common.h"


namespace Mango
{
	class VertexArray : private Utility
	{
	public:
		VertexArray() = default;
		~VertexArray() { Release(); }

		void Setup();
		void Release() override;

		void Bind() const;
		static void Unbind();

		static void EnableAttribute(unsigned int index, size_t count, unsigned int type, bool normalize, size_t stride, size_t offset);

	private:
		unsigned int m_vertex_array = 0xFFFFFFFF;
	};
} // namespace Mango