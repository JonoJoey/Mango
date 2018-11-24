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

	bool operator==(const Block& block) const { return m_is_active == block.m_is_active && m_block_id == block.m_block_id; }

	bool m_is_active;
	BLOCK_ID m_block_id;
};

struct EditedBlock
{
	static uint32_t PackPosition(int x, int y, int z)
	{
		uint32_t pos;
		*reinterpret_cast<uint8_t*>(uintptr_t(&pos)) = uint8_t(x);
		*reinterpret_cast<uint8_t*>(uintptr_t(&pos) + 1) = uint8_t(z);
		*reinterpret_cast<uint16_t*>(uintptr_t(&pos) + 2) = uint16_t(y);
		return pos;
	}
	static void UnpackPosition(uint32_t position, int& x, int& y, int& z)
	{
		x = *reinterpret_cast<uint8_t*>(uintptr_t(&position));
		z = *reinterpret_cast<uint8_t*>(uintptr_t(&position) + 1);
		y = *reinterpret_cast<uint16_t*>(uintptr_t(&position) + 2);
	}

	uint32_t m_position; // relative to chunk origin - 1 byte for x and z, 2 bytes for y
	Block m_block;
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
	Mango::Model* GetModel() { return &m_model; }

	void SetUpdate();

	void SetBlock(int x, int y, int z, const Block& block);
	Block GetBlock(int x, int y, int z);

public:
	static int PositionXToChunk(int x);
	static int PositionZToChunk(int z);

	static uint32_t PackBlock(int x, int y, int z);
	static void UnpackBlock(uint32_t block, int& x, int& y, int& z);

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
	static constexpr int RENDER_CHUNK_RADIUS = 5;
	static constexpr int CHUNK_RELOAD_RADIUS = 10;

public:
	void Setup(glm::fvec3 position);
	void Update(glm::fvec3 position);
	void Release();

	void EditBlock(int x, int y, int z, const Block& block);
	bool GetBlock(int x, int y, int z, Block& block);

	const std::deque<std::shared_ptr<Chunk>>& GetChunks() const { return m_chunks; }
	const std::deque<std::shared_ptr<Chunk>>& GetRenderChunks() const { return m_render_chunks; }
	const std::unordered_map<uint64_t, std::deque<EditedBlock>>& GetEditedBlocks() const { return m_edited_blocks; }

private:
	std::shared_ptr<Chunk> GenerateChunk(int x, int z);
	std::shared_ptr<Chunk> LoadChunk(int x, int z);
	void SaveChunk(std::shared_ptr<Chunk> chunk);
	Chunk* GetChunk(int x, int z);

private:
	siv::PerlinNoise m_perlin_noise;
	std::deque<std::shared_ptr<Chunk>> m_chunks;
	std::deque<std::shared_ptr<Chunk>> m_render_chunks;
	std::unordered_map<uint64_t, std::deque<EditedBlock>> m_edited_blocks;
};