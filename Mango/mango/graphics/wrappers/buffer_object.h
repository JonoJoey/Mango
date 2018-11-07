#pragma once

#include "../../common.h"


namespace Mango
{
	template <unsigned int target>
	class BufferObject : private Utility
	{
	public:
		BufferObject() = default;
		BufferObject(size_t count, size_t element_size, const void* data, unsigned int usage) { Setup(count, element_size, data, usage); }
		~BufferObject() { Release(); }

		void Setup(size_t count, size_t element_size, const void* data, unsigned int usage)
		{
			m_count = count;

			glGenBuffers(1, &m_buffer);
			glBindBuffer(target, m_buffer);
			glBufferData(target, count * element_size, data, usage);
			glBindBuffer(target, 0);
		}
		void Release() override
		{
			if (!IsUtilRegistered())
				return;
			UnregisterUtil();

			if (m_buffer == 0xFFFFFFFF)
				return;

			glDeleteBuffers(1, &m_buffer);
			m_buffer = 0xFFFFFFFF;
		}

		void Bind() const { glBindBuffer(target, m_buffer); }
		static void Unbind() { glBindBuffer(target, 0); }

		unsigned int GetBufferObject() const { return m_buffer; }
		size_t GetCount() const { return m_count; }

	private:
		unsigned int m_buffer = 0xFFFFFFFF;
		size_t m_count = 0;
	};

	typedef BufferObject<GL_ELEMENT_ARRAY_BUFFER> IndexBuffer;
	typedef BufferObject<GL_ARRAY_BUFFER> VertexBuffer;
} // namespace Mango