#include "chunk.h"


void Chunk::Setup()
{
	m_blocks = new uint8_t[WIDTH * HEIGHT * DEPTH];
	memset(m_blocks, 0, WIDTH * HEIGHT * DEPTH);

	m_vbo.Setup(WIDTH * HEIGHT * DEPTH, m_blocks);
}
void Chunk::Release()
{
	if (!m_blocks)
		return;

	delete[] m_blocks;
	m_blocks = nullptr;

	m_vbo.Release();
}
void Chunk::Update()
{
	m_vbo.Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, WIDTH * HEIGHT * DEPTH, m_blocks);
	Mango::VertexBuffer::Unbind();
}