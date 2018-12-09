#include "inventory.h"


bool Inventory::Setup(Mango::MangoCore* mango_core)
{
	for (auto& slot : m_slots)
	{
		slot.m_count = 0;
		slot.m_item_type = 0xFFFFFFFF;
	}

	const auto app_data = Mango::GetAppDataPath();
	if (!m_hotbar_tex.Setup(app_data + "/.mango/resource_packs/default/textures/misc/hotbar.png", true, true, false))
		return false;

	if (!m_selected_box_tex.Setup(app_data + "/.mango/resource_packs/default/textures/misc/selected_box.png", true, true, false))
		return false;

	m_font = mango_core->GetRenderer2D().NewFont("Tahoma", "C:/Windows/Fonts/Tahoma.ttf", 24);

	return true;
}
void Inventory::Release()
{
	m_hotbar_tex.Release();
}
void Inventory::Render(Mango::MangoCore* mango_core, bool inventory_open)
{
	auto renderer_2d = &mango_core->GetRenderer2D();

	renderer_2d->Start();

	// 182, 22
	static constexpr int WIDTH = 182 * 3,
		HEIGHT = 22 * 3,
		BOX_WIDTH = 20 * 3,
		BOX_HEIGHT = 22 * 3;
	const glm::ivec2 pos((mango_core->GetWindowSize()[0] / 2) - (WIDTH / 2), mango_core->GetWindowSize()[1] - HEIGHT);
	const glm::ivec2 pos_2(pos[0] + 3 + (m_selected_slot * BOX_WIDTH), pos[1]);
	
	m_hotbar_tex.Bind();
	renderer_2d->RenderTexturedQuad({ pos[0], pos[1] }, { pos[0] + WIDTH, pos[1] + HEIGHT });

	m_selected_box_tex.Bind();
	renderer_2d->RenderTexturedQuad({ pos_2[0], pos_2[1] }, { pos_2[0] + BOX_WIDTH, pos_2[1] + BOX_HEIGHT });

	renderer_2d->RenderText(m_font, { 50, 50 }, { 1.f, 0.f, 0.f, 1.f }, "test text %i mango", 69);

	renderer_2d->End();
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