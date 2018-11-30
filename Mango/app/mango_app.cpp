#include "mango_app.h"

#include "player.h"


void MangoApp::Run()
{
	// mango core
	if (!m_mango_core.Setup("Mango", { 800, 600 }))
	{
		DBG_ERROR("Failed to setup mango core");
		system("pause");
		return;
	}

	// event handler
	m_event_handler.SetApp(this);
	if (!m_mango_core.RegisterEventHandler(&m_event_handler))
	{
		DBG_ERROR("Failed to setup event handler");
		system("pause");
		return;
	}

	// input handler
	if (!m_mango_core.RegisterEventHandler(&m_input_handler))
	{
		DBG_ERROR("Failed to setup input handler");
		system("pause");
		return;
	}

	// setup imgui style
	{
		auto& style = ImGui::GetStyle();
		style.WindowRounding = 2;
		style.ChildRounding = 0;
		style.FrameRounding = 2;
		style.ScrollbarRounding = 0;
		style.GrabRounding = 0;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.78f, 0.47f, 0.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.12f, 0.16f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.12f, 0.16f, 0.39f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.12f, 0.16f, 0.59f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.12f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.27f, 0.20f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 0.00f, 0.08f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.27f, 0.20f, 0.00f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.36f, 0.17f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.65f, 0.50f, 0.23f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.59f, 0.36f, 0.17f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.59f, 0.36f, 0.17f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.65f, 0.50f, 0.23f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.12f, 0.16f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.12f, 0.16f, 0.39f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.12f, 0.16f, 0.47f);
		colors[ImGuiCol_Header] = ImVec4(0.16f, 0.12f, 0.00f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.27f, 0.20f, 0.00f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.27f, 0.20f, 0.00f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	// framebuffer
	m_framebuffer.Setup(m_mango_core.GetWindowSize());

	float accumulated_time = 0.f;
	m_tick_count = 0;

	OnInit();

	// loooooooooooop
	while (m_mango_core.NextFrame())
	{
		const float frame_time = m_mango_core.GetFrameTime();

		accumulated_time += frame_time;
		while (accumulated_time > m_interval_per_tick)
		{
			accumulated_time -= m_interval_per_tick;
			OnTick();
			m_tick_count++;
		}

		OnFrame(frame_time, accumulated_time / m_interval_per_tick);

		m_mango_core.EndFrame();
	}

	OnRelease();

	// release
	m_mango_core.Release();
}

void MangoApp::OnInit()
{
	m_mango_core.SetVerticalSync(false);

	Mango::RescourcePool<Mango::Texture>::Get()->AddRes("mango", "res/textures/mango.png", true, true);

	m_block_map["null"] = 0;
	m_block_map["cobblestone"] = 1;
	m_block_map["grass"] = 2;
	m_block_map["diamond_ore"] = 3;
	m_block_map["iron_ore"] = 4;
	m_block_map["oak_log"] = 5;
	m_block_map["oak_plank"] = 6;
	m_block_map["mossy_cobblestone"] = 7;

	if (!World::DoesWorldExist("res/worlds/test_world"))
		World::CreateNewWorld("res/worlds/test_world", 69);

	m_world.Setup(&m_mango_core, "res/worlds/test_world", m_block_map);

	m_local_player = m_world.AddEntity<LocalPlayer>(this);
	m_local_player->SetPosition({ 0.f, 250.f, 0.f });

	Mango::DiscordRPC::Setup("514257473654489098");
	Mango::DiscordRPC::Update("you are", "a noob", "mango", "mAnGo", "m_fancy", "MaNgO", Mango::DiscordRPC::GetStartTime(), 0);
}
void MangoApp::OnRelease()
{
	Mango::DiscordRPC::Release();

	m_world.Release();
}

void MangoApp::OnTick()
{
	m_world.Update(m_mango_core.GetRenderer3D().GetCamera().GetPosition());
}
void MangoApp::OnFrame(float frame_time, float lerp)
{
	auto& renderer_2d = m_mango_core.GetRenderer2D();
	auto& renderer_3d = m_mango_core.GetRenderer3D();

	m_framebuffer.Bind();
	m_mango_core.Clear({ 0.f, 0.f, 0.f });

	// render entire world and stuffs
	m_world.Render(&m_mango_core, lerp);

	// 2d rendering
	{
		renderer_2d.Start();

		Mango::RescourcePool<Mango::Texture>::Get()->GetRes("mango")->Bind();
		renderer_2d.RenderTexturedQuad(m_mango_core.GetMousePosition() - 10.f, m_mango_core.GetMousePosition() + 10.f);

		renderer_2d.End();
	}

	// render final mango (post-process)
	{
		Mango::Framebuffer::Unbind();
		m_mango_core.Clear({ 0.f, 0.f, 0.f });

		static const unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		static const float positions[] =
		{
			-1.f, 1.f,  // top left
			-1.f, -1.f, // bottom left
			1.f, -1.f,  // bottom right
			1.f, 1.f    // top right
		};
		static const float tex_coords[] =
		{
			0.f, 1.f,
			0.f, 0.f,
			1.f, 0.f,
			1.f, 1.f
		};

		// initialize
		if (static bool is_init = false; !is_init)
		{
			is_init = true;

			Mango::RescourcePool<Mango::Shader>::Get()->GetOrAddRes("post_process_shader",
				Mango::Shader::ReadFile("res/shaders/post_process_vs.glsl"),
				Mango::Shader::ReadFile("res/shaders/post_process_fs.glsl"));

			auto quad_model = Mango::RescourcePool<Mango::Model>::Get()->AddRes("post_process_shader",
				GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

			quad_model->GetVAO().Bind();

			// positions
			auto* vbo = &quad_model->AddVBO();
			vbo->Setup(2 * 4 * sizeof(float), positions, GL_STATIC_DRAW);
			vbo->Bind();
			Mango::VertexArray::EnableAttribute(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

			// tex_coords
			vbo = &quad_model->AddVBO();
			vbo->Setup(2 * 4 * sizeof(float), tex_coords, GL_STATIC_DRAW);
			vbo->Bind();
			Mango::VertexArray::EnableAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
		}

		auto post_process_shader = Mango::RescourcePool<Mango::Shader>::Get()->GetRes("post_process_shader");
		auto quad_model = Mango::RescourcePool<Mango::Model>::Get()->GetRes("post_process_shader");

		renderer_2d.Start();

		// texture shader
		post_process_shader->Bind();
		quad_model->GetVAO().Bind();

		m_framebuffer.GetTexture().Bind();
		glDrawElements(quad_model->GetMode(), quad_model->GetIBO().GetCount(), quad_model->GetIBO().GetType(), nullptr);

		Mango::Shader::Unbind();
		Mango::VertexArray::Unbind();

		renderer_2d.End();
	}

	// imgui
	{
		ImGui::Begin("Mango");

		ImGui::Text("FPS: ");
		ImGui::SameLine(0.f, 0.f);
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "%4.0f", 1.f / m_mango_core.GetFrameTime());
		ImGui::SameLine();
		ImGui::Text("Tickrate: ");
		ImGui::SameLine(0.f, 0.f);
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "%.0f", 1.f / m_interval_per_tick);

		ImGui::Text("Position: ");
		ImGui::SameLine(0.f, 0.f);
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "%.2f %.2f %.2f", renderer_3d.GetCamera().GetPosition().x, renderer_3d.GetCamera().GetPosition().y, renderer_3d.GetCamera().GetPosition().z);

		ImGui::Text("Chunk: ");
		ImGui::SameLine(0.f, 0.f);
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "%i %i", Chunk::PositionXToChunk(int(renderer_3d.GetCamera().GetPosition().x)), Chunk::PositionZToChunk(int(renderer_3d.GetCamera().GetPosition().z)));

		ImGui::Text("Chunks: ");
		ImGui::SameLine(0.f, 0.f);
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "%i", m_world.GetChunks().size());

		ImGui::Text("Rendered chunks: ");
		ImGui::SameLine(0.f, 0.f);
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "%i", m_world.GetRenderChunks().size());

		ImGui::Text("Used memory: ");
		ImGui::SameLine(0.f, 0.f);
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "%.1f MB", (m_world.GetRenderChunks().size() * sizeof(Block) * Chunk::WIDTH * Chunk::HEIGHT * Chunk::DEPTH) / 1000000.f);

		size_t edited_blocks_size = 0;
		for (const auto& chunk : m_world.GetEditedBlocks())
			edited_blocks_size += chunk.second.size();

		ImGui::Text("Edited blocks: ");
		ImGui::SameLine(0.f, 0.f);
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "%i", edited_blocks_size);

		if (static bool c = false; ImGui::Checkbox("Vertical Sync", &c))
			m_mango_core.SetVerticalSync(c);

		ImGui::SameLine();
		if (static bool c = m_local_player->IsThirdPerson(); ImGui::Checkbox("Thirdperson", &c))
			m_local_player->SetThirdPerson(c);

		if (static float tmp = m_local_player->GetThirdPersonDistance(); ImGui::SliderFloat("Thirdperson distance", &tmp, 0.f, 100.f))
			m_local_player->SetThirdPersonDistance(tmp);

		ImGui::SliderFloat("Mouse sensitivity", &m_mouse_sensitivity, 0.01f, 2.f);
		if (static int render_distance = m_world.GetRenderDistance(); ImGui::SliderInt("Render distance", &render_distance, 1, 64))
			m_world.SetRenderDistance(render_distance);

		std::string selected_block = m_local_player->GetSelectedBlock();
		if (ImGui::BeginCombo("Block##combo", selected_block.c_str()))
		{
			for (const auto& block : m_block_map)
			{
				bool is_selected = selected_block == block.first;
				if (ImGui::Selectable(block.first.c_str(), is_selected))
					m_local_player->SetSelectedBlock(block.first);
			}

			ImGui::EndCombo();
		}

		ImGui::End();
	}
}