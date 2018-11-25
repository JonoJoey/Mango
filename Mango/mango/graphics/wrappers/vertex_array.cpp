#include "vertex_array.h"


namespace Mango
{
	void VertexArray::Setup()
	{
		glGenVertexArrays(1, &m_vertex_array);
	}
	void VertexArray::Release()
	{
		if (m_vertex_array == 0xFFFFFFFF)
			return;

		glDeleteVertexArrays(1, &m_vertex_array);
		m_vertex_array = 0xFFFFFFFF;
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_vertex_array);
	}
	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	void VertexArray::EnableAttribute(unsigned int index, size_t count, unsigned int type, bool normalize, size_t stride, size_t offset, unsigned int diviser)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, count, type, normalize, stride, reinterpret_cast<void*>(offset));
		
		if (diviser)
			glVertexAttribDivisor(index, diviser);
	}
	void VertexArray::EnableAttributeInt(unsigned int index, size_t count, unsigned int type, size_t stride, size_t offset, unsigned int diviser)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribIPointer(index, count, type, stride, reinterpret_cast<void*>(offset));

		if (diviser)
			glVertexAttribDivisor(index, diviser);
	}
} // namespace Mango