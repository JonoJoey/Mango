#pragma once

#include "../mango/mango.h"


typedef uint32_t ITEM_ID;
typedef uint16_t BLOCK_ID;


uint64_t PackChunk(int x, int z);
void UnpackChunk(uint64_t chunk, int& x, int& z);


struct ItemStack
{
	ITEM_ID m_item_id;
	size_t m_count;
};

struct Block
{
	static Block Create(BLOCK_ID id)
	{
		return Block({ true, id });
	}
	static Block Inactive()
	{
		return Block({ false, 0 });
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