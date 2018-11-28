#pragma once

#include "../../common.h"


namespace Mango
{
	template <unsigned int target>
	class BufferObject : private Utility
	{
	public:
		BufferObject() = default;
		BufferObject(size_t size, const void* data, unsigned int usage = GL_DYNAMIC_DRAW) { Setup(size, data, usage); }
		~BufferObject() { Release(); UnregisterUtil(); }

		void Setup(size_t size, const void* data, unsigned int usage = GL_DYNAMIC_DRAW)
		{
			glGenBuffers(1, &m_buffer);
			glBindBuffer(target, m_buffer);
			glBufferData(target, size, data, usage);
			glBindBuffer(target, 0);
		}
		void Release() override
		{
			if (m_buffer == 0xFFFFFFFF)
				return;

			glDeleteBuffers(1, &m_buffer);
			m_buffer = 0xFFFFFFFF;
		}

		void Bind() const { glBindBuffer(target, m_buffer); }
		static void Unbind() { glBindBuffer(target, 0); }

		unsigned int& GetBufferObject() { return m_buffer; }
		unsigned int GetBufferObject() const { return m_buffer; }

	private:
		unsigned int m_buffer = 0xFFFFFFFF;
	};

	class IndexBuffer : public BufferObject<GL_ELEMENT_ARRAY_BUFFER>
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(size_t count, unsigned int index_type, const void* data, unsigned int usage = GL_DYNAMIC_DRAW) { Setup(count, index_type, data, usage); }

		void Setup(size_t count, unsigned int index_type, const void* data, unsigned int usage = GL_DYNAMIC_DRAW)
		{
			m_count = count;
			m_type = index_type;

			glGenBuffers(1, &GetBufferObject());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetBufferObject());
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * GetElementSize(index_type), data, usage);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void SetCount(size_t count) { m_count = count; }
		size_t GetCount() const { return m_count; }
		unsigned int GetType() const { return m_type; }

	private:
		size_t m_count;
		unsigned int m_type;
	};

	typedef BufferObject<GL_ARRAY_BUFFER> VertexBuffer;
} // namespace Mango