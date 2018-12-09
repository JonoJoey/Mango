#include "item_map.h"

#include <filesystem>


bool ItemMap::CreateTextures(const std::string& resource_pack)
{
	ItemMapElement element;
	element.m_name = "null";
	element.m_texture_name = "null.png";
	element.m_max_stack = 64;
	element.m_item_id = 0;
	element.m_block_id = 0;
	m_item_map["null"] = element;

	for (auto& item : m_item_map)
	{
		auto path = resource_pack + "/textures/items/gui/" + item.second.m_texture_name;
		if (!std::filesystem::exists(path))
		{
			DBG_ERROR("Failed to find texture - \"%s\"", path.c_str());
			path = resource_pack + "/textures/items/gui/null.png";
		}

		if (!item.second.m_gui_texture.Setup(path, true, true, false))
			DBG_ERROR("Failed to setup texture - \"%s\"", path.c_str());
	}

	return true;
}
void ItemMap::Release()
{
	m_item_map.clear();
}

void ItemMap::AddItem(const std::string& item_name, const std::string& texture_name, ITEM_ID item_id, size_t max_stack, BLOCK_ID block_id)
{
	if (m_item_map.find(item_name) != m_item_map.end())
		DBG_LOG("Replaced item: \"%s\"", item_name.c_str());

	ItemMapElement element;
	element.m_name = item_name;
	element.m_texture_name = texture_name;
	element.m_max_stack = max_stack;
	element.m_item_id = item_id;
	element.m_block_id = block_id;
	m_item_map[item_name] = element;
}
const ItemMapElement* ItemMap::GetItem(const std::string& item_name) const
{
	if (auto it = m_item_map.find(item_name); it == m_item_map.end())
		return &m_item_map.at("null");
	else
		return &it->second;
}
const ItemMapElement* ItemMap::GetItem(ITEM_ID item_id) const
{
	for (auto it = m_item_map.begin(); it != m_item_map.end(); it++)
	{
		if (it->second.m_item_id == item_id)
			return &it->second;
	}

	return &m_item_map.at("null");
}
const ItemMapElement* ItemMap::GetItemFromBlock(BLOCK_ID block_id) const
{
	for (auto it = m_item_map.begin(); it != m_item_map.end(); it++)
	{
		if (it->second.m_block_id == block_id)
			return &it->second;
	}

	return &m_item_map.at("null");
}