#pragma once

#include "../common.h"

#include "wrappers/vertex_array.h"
#include "wrappers/buffer_object.h"


namespace Mango
{
	// mearly a container
	class Model : private Utility
	{
	public:
		Model() = default;
		Model(unsigned int mode      /* ex. GL_TRIANGLES    */, size_t index_count,
			unsigned int index_type  /* ex. GL_UNSIGNED_INT */, const void* indices)
		{
			Setup(mode, index_count, index_type, indices);
		}
		~Model() { Release(); UnregisterUtil(); }

		void Setup(unsigned int mode /* ex. GL_TRIANGLES    */, size_t index_count, 
			unsigned int index_type  /* ex. GL_UNSIGNED_INT */, const void* indices);
		void Release() override;

		unsigned int GetMode() const { return m_mode; }

		VertexArray& GetVAO() { return m_vao; }
		const VertexArray& GetVAO() const { return m_vao; }

		IndexBuffer& GetIBO() { return m_ibo; }
		const IndexBuffer& GetIBO() const { return m_ibo; }

		std::deque<VertexBuffer>& GetVBOs() { return m_vbos; }
		VertexBuffer& AddVBO() { return m_vbos.emplace_back(); }

	private:
		unsigned int m_mode = 0xFFFFFFFF;
		VertexArray m_vao;
		IndexBuffer m_ibo;
		std::deque<VertexBuffer> m_vbos;
	};
} // namespace Mango