#pragma once

#include "../mango/mango.h"
#include "defines.h"


struct BlockMapElement
{
	BLOCK_ID m_block_id;
	ITEM_ID m_item_id;
	std::array<uint32_t, 6> m_face_indices;
	std::array<std::string, 6> m_texture_names;
};

class BlockMap
{
public:
	BlockMap() { Release(); }
	~BlockMap() { Release(); }
	BlockMap(const BlockMap& block_map) { m_block_map = block_map.GetBlockMap(); }
	void operator=(const BlockMap& block_map) { m_block_map = block_map.GetBlockMap(); }

	bool CreateTextures(const std::string& resource_pack);
	void Release();

	void AddBlock(const std::string& block_name, BLOCK_ID block_id, ITEM_ID item_id, const std::array<std::string, 6>& face_textures);
	void RemoveBlock(const std::string& block_name);
	BlockMapElement GetBlock(const std::string& block_name) const;
	BlockMapElement GetBlock(BLOCK_ID block_id) const;

	const std::unordered_map<std::string, BlockMapElement>& GetBlockMap() const { return m_block_map; }
	const std::deque<Mango::TextureArray>& GetTextureArrays() const { return m_texture_arrays; }

private:
	std::unordered_map<std::string, BlockMapElement> m_block_map;
	std::deque<Mango::TextureArray> m_texture_arrays; // each texture array goes up to 256
};