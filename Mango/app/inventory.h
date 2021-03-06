#pragma once

#include "../mango/mango.h"

#include "defines.h"


class MangoApp;
class Inventory
{
public:
	bool Setup(std::string resource_pack);
	void Release();
	void Render(MangoApp* mango_app, bool inventory_open);

	void AddItems(ITEM_ID item_id, size_t count, size_t max_count);
	size_t RemoveItems(size_t slot, size_t count);
	const std::array<ItemStack, 9 * 4>& GetSlots() const { return m_slots; }

	void SetSelectedSlot(size_t slot) { m_selected_slot = std::clamp<uint32_t>(slot, 0, 9); }
	size_t GetSelectedSlot() const { return m_selected_slot; }
	ItemStack GetSelectedItem() const { return m_slots[m_selected_slot]; }

private:
	size_t GetNextEmptySlot();
	size_t FindSlotValid(ITEM_ID item_id, size_t max_count);

private:
	std::array<ItemStack, 9 * 4> m_slots;
	size_t m_selected_slot = 0;

	Mango::Texture m_hotbar_tex,
		m_selected_box_tex;
	ImFont* m_font = nullptr;
};