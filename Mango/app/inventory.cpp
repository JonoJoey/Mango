#include "inventory.h"


Inventory::Inventory()
{
	for (auto& slot : m_slots)
	{
		slot.m_count = 0;
		slot.m_item_type = 0xFFFFFFFF;
	}
}

void Inventory::Render(Mango::MangoCore* mango_core, bool inventory_open)
{
	for (size_t i = 0; i < 9; i++)
	{
		if (m_slots[i].m_count > 0)
			DBG_LOG("%i - %i", m_slots[i].m_item_type, m_slots[i].m_count);
	}
}

void Inventory::AddItems(ITEM_ID item_id, size_t count)
{
	auto slot = FindSlotValid(item_id);
	if (slot == 0xFFFFFFFF)
	{
		slot = GetNextEmptySlot();
		if (slot == 0xFFFFFFFF)
			return;
	}

	auto add = std::min(count, (64 - m_slots[slot].m_count));
	m_slots[slot].m_count += add;
	m_slots[slot].m_item_type = item_id;
	count -= add;

	while (count > 0)
	{
		slot = GetNextEmptySlot();

		// inventory full
		if (slot == 0xFFFFFFFF)
			return;

		add = count - (64 - m_slots[slot].m_count);
		m_slots[slot].m_item_type = item_id;
		m_slots[slot].m_count += add;
		count -= add;
	}
}
size_t Inventory::RemoveItems(size_t slot, size_t count)
{
	auto c = m_slots[slot].m_count;
	if (c <= 0)
		return 0;

	if (count <= c)
	{
		m_slots[slot].m_count -= count;
		return count;
	}
	else
	{
		m_slots[slot].m_count = 0;
		return c;
	}
}

size_t Inventory::GetNextEmptySlot()
{
	for (size_t i = 0; i < m_slots.size(); i++)
	{
		if (m_slots[i].m_count <= 0)
			return i;
	}

	return 0xFFFFFFFF;
}
size_t Inventory::FindSlotValid(ITEM_ID item_id)
{
	for (size_t i = 0; i < m_slots.size(); i++)
	{
		if (m_slots[i].m_item_type == item_id && m_slots[i].m_count > 0 && m_slots[i].m_count < 64)
			return i;
	}

	return 0xFFFFFFFF;
}