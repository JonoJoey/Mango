#include "block_map.h"

#include <filesystem>


bool BlockMap::CreateTextures(const std::string& texture_path)
{
	m_texture_arrays.clear();

	std::deque<std::string> unique_textures;
	const auto FindElement = [](const std::string& name, std::deque<std::string>& deq) -> size_t
	{
		for (size_t i = 0; i < deq.size(); i++)
		{
			if (deq[i] == name)
				return i;
		}

		return 0xFFFFFFFF;
	};

	for (auto& block : m_block_map)
	{
		for (size_t i = 0; i < 6; i++)
		{
			const auto texture_name = block.second.m_texture_names[i];
			if (const auto index = FindElement(texture_name, unique_textures); index == 0xFFFFFFFF)
			{
				if (!std::filesystem::exists(texture_path + "/" + texture_name))
				{
					block.second.m_face_indices[i] = 0;
					DBG_ERROR("Failed to find texture: \"%s\"", (texture_path + "/" + texture_name).c_str());
				}
				else
				{
					block.second.m_face_indices[i] = unique_textures.size();
					unique_textures.push_back(texture_name);
				}
			}
			else
				block.second.m_face_indices[i] = index;
		}
	}
	
	for (size_t i = 0; i < unique_textures.size(); i += 256)
	{
		size_t amount = std::min(256U, unique_textures.size() - i);
		std::vector<std::string> textures_chunk;
		for (size_t x = 0; x < amount; x++)
			textures_chunk.push_back(texture_path + "/" + unique_textures[i + x]);

		m_texture_arrays.emplace_back();
		if (!m_texture_arrays.back().Setup(textures_chunk, glm::ivec2(16, 16), true, false, false, -1.f))
		{
			m_texture_arrays.clear();
			DBG_ERROR("Failed to load texture array");
			return false;
		}
	}

	return true;
}
void BlockMap::Release()
{
	m_texture_arrays.clear();
	m_block_map.clear();

	BlockMapElement element;
	element.m_block_id = 0;
	for (auto& texture_name : element.m_texture_names)
		texture_name = "null.png";
	m_block_map["null"] = element;
}

void BlockMap::AddBlock(const std::string& block_name, BLOCK_ID block_id, ITEM_ID item_id, const std::array<std::string, 6>& face_textures)
{
	// cant override this block dumb nut
	if (block_name == "null")
		return;

	// block already exists, erase it
	if (m_block_map.find(block_name) == m_block_map.end())
		m_block_map.erase(block_name);

	BlockMapElement element;
	element.m_block_id = block_id;
	element.m_item_id = item_id;
	element.m_texture_names = face_textures;
	m_block_map[block_name] = element;
}
void BlockMap::RemoveBlock(const std::string& block_name)
{
	if (block_name == "null")
		return;

	if (m_block_map.find(block_name) != m_block_map.end())
		m_block_map.erase(block_name);
}

BlockMapElement BlockMap::GetBlock(const std::string& block_name) const
{
	if (auto it = m_block_map.find(block_name); it != m_block_map.end())
		return it->second;

	return m_block_map.at("null");
}
BlockMapElement BlockMap::GetBlock(BLOCK_ID block_id) const
{
	for (auto it = m_block_map.begin(); it != m_block_map.end(); it++)
	{
		if (it->second.m_block_id == block_id)
			return it->second;
	}

	return m_block_map.at("null");
}
