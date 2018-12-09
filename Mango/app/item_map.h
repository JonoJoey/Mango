#pragma once

#include "../mango/mango.h"

#include "defines.h"


struct ItemMapElement
{
	std::string m_name,
		m_texture_name;
	size_t m_max_stack;
	ITEM_ID m_item_id;
	BLOCK_ID m_block_id = BLOCK_ID(-1);
	Mango::Texture m_gui_texture;
};

class ItemMap
{
public:
	ItemMap() = default;
	~ItemMap() { Release(); }

	bool CreateTextures(const std::string& resource_pack);
	void Release();

	void AddItem(const std::string& item_name, const std::string& texture_name, ITEM_ID item_id, size_t max_stack = 64, BLOCK_ID block_id = BLOCK_ID(-1));
	const ItemMapElement* GetItem(const std::string& item_name) const;
	const ItemMapElement* GetItem(ITEM_ID item_id) const;
	const ItemMapElement* GetItemFromBlock(BLOCK_ID block_id) const;

private:
	std::unordered_map<std::string, ItemMapElement> m_item_map;
};