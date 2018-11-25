#include "model.h"


namespace Mango
{
	void Model::Setup(unsigned int mode, size_t index_count, unsigned int index_type, const void* indices)
	{
		m_mode = mode;

		m_vao.Setup();
		m_vao.Bind();

		m_ibo.Setup(index_count, index_type, indices, GL_STATIC_DRAW);
		m_ibo.Bind();

		VertexArray::Unbind();
	}
	void Model::Release()
	{
		if (m_mode == 0xFFFFFFFF)
			return;

		m_mode = 0xFFFFFFFF;
		m_vao.Release();
		m_ibo.Release();
	}
} // namespace Mango