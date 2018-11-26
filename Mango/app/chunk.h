#pragma once

#include "block.h"

// index = x + (y * WIDTH) + (z * WIDTH * HEIGHT);
// x = i % WIDTH;
// y = (i / WIDTH) % HEIGHT;
// z = i / (WIDTH * HEIGHT);


class Chunk
{
public:
	static constexpr int WIDTH = 16;
	static constexpr int HEIGHT = 256;
	static constexpr int DEPTH = 16;

public:
	Chunk(int x, int z) { Setup(x, z); }
	~Chunk() { Release(); }

	void Setup(int x, int z);
	void Release();
	bool Update(std::unordered_map<uint64_t, std::shared_ptr<Chunk>> chunks);

	int GetX() const { return m_x; }
	int GetZ() const { return m_z; }
	Mango::Model* GetModel() { return &m_model; }

	void SetBlock(int x, int y, int z, const Block& block);
	Block GetBlock(int x, int y, int z);

	void SetNeedsUpdate(bool value) { m_needs_update = value; }
	bool DoesNeedUpdate() const { return m_needs_update; }

public:
	static int PositionXToChunk(int x);
	static int PositionZToChunk(int z);

	static uint32_t PackBlock(int x, int y, int z);
	static void UnpackBlock(uint32_t block, int& x, int& y, int& z);

private:
	Block* m_blocks;
	Mango::Model m_model;
	int m_x = 0,
		m_z = 0;
	bool m_needs_update = false;
};