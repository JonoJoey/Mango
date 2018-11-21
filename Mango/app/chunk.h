#pragma once

#include "../mango/mango.h"


// index = x + (y * WIDTH) + (z * WIDTH * HEIGHT);
// x = i % WIDTH;
// y = (i / WIDTH) % HEIGHT;
// z = i / (WIDTH * HEIGHT);

class Chunk
{
public:
	static constexpr size_t WIDTH = 16;
	static constexpr size_t HEIGHT = 256;
	static constexpr size_t DEPTH = 16;

public:
	Chunk() = default;
	~Chunk() { Release(); }

	void Setup();
	void Release();
	void Update();

	uint8_t* GetBlocks() { return m_blocks; }
	uint8_t GetBlock(size_t x, size_t y, size_t z) const { return m_blocks[x + (y * WIDTH) + (z * WIDTH * HEIGHT)]; }
	void SetBlock(size_t x, size_t y, size_t z, uint8_t value) { m_blocks[x + (y * WIDTH) + (z * WIDTH * HEIGHT)] = value; }

	Mango::VertexBuffer* GetVBO() { return &m_vbo; }

private:
	uint8_t* m_blocks = nullptr;
	Mango::VertexBuffer m_vbo;
};