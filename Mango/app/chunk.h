#pragma once

#include "../mango/mango.h"


// index = x + (y * WIDTH) + (z * WIDTH * HEIGHT);
// x = i % WIDTH;
// y = (i / WIDTH) % HEIGHT;
// z = i / (WIDTH * HEIGHT);

typedef uint16_t BLOCK_ID;


struct Block
{
	static Block Create(BLOCK_ID id)
	{
		return Block({ true, id });
	}
	static Block Inactive()
	{
		return Block({ false });
	}

	bool m_is_active;
	BLOCK_ID m_block_id;
};

class Chunk
{
public:
	static constexpr int WIDTH = 16;
	static constexpr int HEIGHT = 256;
	static constexpr int DEPTH = 16;

public:
	Chunk() = default;
	Chunk(int x, int z) { Setup(x, z); }
	~Chunk() { Release(); }

	void Setup(int x, int z);
	void Release();
	bool Update(std::deque<std::shared_ptr<Chunk>>& render_chunks);

	int GetX() const { return m_x; }
	int GetZ() const { return m_z; }

	void SetUpdate() { m_needs_update = true; }
	void SetBlock(int x, int y, int z, const Block& block) { m_needs_update = true; m_blocks[x + (y * WIDTH) + (z * WIDTH * HEIGHT)] = block; }
	Block GetBlock(int x, int y, int z) { return m_blocks[x + (y * WIDTH) + (z * WIDTH * HEIGHT)]; }
	Mango::Model* GetModel() { return &m_model; }

	static int PositionXToChunk(int x);
	static int PositionZToChunk(int z);

private:
	Block* m_blocks = nullptr;
	Mango::Model m_model;
	int m_x = 0,
		m_z = 0;
	bool m_needs_update = false;
};

class World
{
public:
	static constexpr int CHUNK_RADIUS = 5;

public:
	void Setup(glm::fvec3 position);
	void Update(glm::fvec3 position);
	void Release();

	void EditBlock(int x, int y, int z, const Block& block);

	std::deque<std::shared_ptr<Chunk>>& GetChunks() { return m_chunks; }
	std::deque<std::shared_ptr<Chunk>>& GetRenderChunks() { return m_render_chunks; }

private:
	std::shared_ptr<Chunk> GenerateChunk(int x, int z);
	Chunk* GetChunk(int x, int z);

private:
	siv::PerlinNoise m_perlin_noise;
	std::deque<std::shared_ptr<Chunk>> m_chunks;
	std::deque<std::shared_ptr<Chunk>> m_render_chunks;
};