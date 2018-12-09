#include "inventory.h"

#include "mango_app.h"


bool Inventory::Setup(std::string resource_pack)
{
	for (auto& slot : m_slots)
	{
		slot.m_count = 0;
		slot.m_item_id = ITEM_ID(-1);
	}

	const auto app_data = Mango::GetAppDataPath();
	if (!m_hotbar_tex.Setup(resource_pack + "/textures/misc/hotbar.png", true, true, false))
		return false;

	if (!m_selected_box_tex.Setup(resource_pack + "/textures/misc/selected_box.png", true, true, false))
		return false;

	if (!m_font)
		m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF((resource_pack + "/fonts/minecraft_font.ttf").c_str(), 32);

	return true;
}
void Inventory::Release()
{
	m_hotbar_tex.Release();
	m_selected_box_tex.Release();
}
void Inventory::Render(MangoApp* mango_app, bool inventory_open)
{
	auto mango_core = mango_app->GetMangoCore();
	auto renderer_2d = &mango_core->GetRenderer2D();

	renderer_2d->Start();

	// 182, 22
	static constexpr int WIDTH = 182 * 3,
		HEIGHT = 22 * 3,
		BOX_WIDTH = 22 * 3,
		BOX_HEIGHT = 22 * 3;
	const glm::ivec2 pos((mango_core->GetWindowSize()[0] / 2) - (WIDTH / 2), mango_core->GetWindowSize()[1] - HEIGHT);
	const glm::ivec2 pos_2(pos[0] + (m_selected_slot * (WIDTH / 9)), pos[1]);
	
	m_hotbar_tex.Bind();
	renderer_2d->RenderTexturedQuad({ pos[0], pos[1] }, { pos[0] + WIDTH, pos[1] + HEIGHT });

	m_selected_box_tex.Bind();
	renderer_2d->RenderTexturedQuad({ pos_2[0] - 1, pos_2[1] - 1 }, { pos_2[0] + BOX_WIDTH + 2, pos_2[1] + BOX_HEIGHT + 2 });

	for (size_t i = 0; i < 9; i++)
	{
		if (m_slots[i].m_count <= 0)
			continue;

		auto item_pos = glm::ivec2(pos[0] + (i * (WIDTH / 9)), pos[1]);
		auto item = mango_app->GetWorld()->GetItemMap()->GetItem(m_slots[i].m_item_id);

		item->m_gui_texture.Bind();
		renderer_2d->RenderTexturedQuad({ item_pos[0] + 16, item_pos[1] + 16 }, { item_pos[0] + BOX_WIDTH - 16, item_pos[1] + BOX_HEIGHT - 16 });
	}

	// tfw u dont wanna make a text renderer
	{
		ImGui::PushFont(m_font);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize({ float(mango_core->GetWindowSize()[0]), float(mango_core->GetWindowSize()[1]) });
		ImGui::SetNextWindowBgAlpha(0.f);
		ImGui::Begin("##inventory drawing window", nullptr, ImGuiWindowFlags_NoTitleBar | 
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | 
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		for (size_t i = 0; i < 9; i++)
		{
			if (m_slots[i].m_count <= 1)
				continue;

			const auto text = std::to_string(m_slots[i].m_count);
			const auto text_size = ImGui::CalcTextSize(text.c_str());
			const auto text_pos = ImVec2(float(pos[0] + (i * (WIDTH / 9)) + (BOX_WIDTH - (text_size[0] + 6))), float(pos[1] + (BOX_HEIGHT / 2) - 6));

			draw_list->AddText(ImVec2(text_pos[0] + 2, text_pos[1] + 2), 0xBB000000, text.c_str());
			draw_list->AddText(text_pos, 0xFFFFFFFF, text.c_str());
		}

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopFont();
	}

	renderer_2d->End();
}

void Inventory::AddItems(ITEM_ID item_id, size_t count, size_t max_count)
{
	auto slot = FindSlotValid(item_id, max_count);
	if (slot == 0xFFFFFFFF)
	{
		slot = GetNextEmptySlot();
		if (slot == 0xFFFFFFFF)
			return;
	}

	auto add = std::min(count, (max_count - m_slots[slot].m_count));
	m_slots[slot].m_count += add;
	m_slots[slot].m_item_id = item_id;
	count -= add;

	while (count > 0)
	{
		slot = GetNextEmptySlot();

		// inventory full
		if (slot == 0xFFFFFFFF)
			return;

		add = count - (max_count - m_slots[slot].m_count);
		m_slots[slot].m_item_id = item_id;
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
size_t Inventory::FindSlotValid(ITEM_ID item_id, size_t max_count)
{
	for (size_t i = 0; i < m_slots.size(); i++)
	{
		if (m_slots[i].m_item_id == item_id && m_slots[i].m_count > 0 && m_slots[i].m_count < max_count)
			return i;
	}

	return 0xFFFFFFFF;
}