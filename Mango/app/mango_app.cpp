#include "mango_app.h"




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
		}

		OnFrame(frame_time, accumulated_time / m_interval_per_tick);

		m_mango_core.EndFrame();
	}

	// release
	m_mango_core.Release();
}

void MangoApp::OnInit()
{
	Mango::RescourcePool<Mango::Texture>::Get()->AddRes("mango", "res/textures/mango.png", true, true);

	Mango::RescourcePool<Mango::Shader>::Get()->AddRes("cube_shader", 
		Mango::Shader::ReadFile("res/shaders/cube_vs.glsl"), 
		Mango::Shader::ReadFile("res/shaders/cube_fs.glsl"));

	auto cube_model = Mango::RescourcePool<Mango::Model>::Get()->AddRes("cube");
	Mango::LoadCubeModel(*cube_model);

	std::string block_names_to_add[] =
	{
		"grass",
		"cobblestone",
		"emerald_ore",
		"diamond_ore",
		"crafting_table"
	};

	m_block_names.push_back("none");
	for (auto block_name : block_names_to_add)
	{
		Mango::RescourcePool<Mango::CubeTexture>::Get()->AddRes(block_name, std::array<std::string, 6>({
			"res/textures/blocks/" + block_name + "/side.png",
			"res/textures/blocks/" + block_name + "/side.png",
			"res/textures/blocks/" + block_name + "/top.png",
			"res/textures/blocks/" + block_name + "/bottom.png",
			"res/textures/blocks/" + block_name + "/side.png",
			"res/textures/blocks/" + block_name + "/side.png",
			}), false, false, false);

		m_block_names.push_back(block_name);
	}

	m_chunk.Setup();

	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			for (int z = 0; z < 16; z++)
			{
				m_chunk.SetBlock(x, y, z, uint8_t((x + y + z) % m_block_names.size()));
			}
		}
	}

	m_chunk.Update();

	m_camera.SetPosition({ 0.f, 0.f, 2.f });

	Mango::DiscordRPC::Setup("514257473654489098");
	Mango::DiscordRPC::Update("you are", "a noob", "mango", "mAnGo", "m_fancy", "MaNgO", Mango::DiscordRPC::GetStartTime(), 0);
}
void MangoApp::OnRelease()
{
	Mango::DiscordRPC::Release();

	m_chunk.Release();

	Mango::RescourcePool<Mango::Model>::Get()->Release();
	Mango::RescourcePool<Mango::Shader>::Get()->Release();
	Mango::RescourcePool<Mango::Texture>::Get()->Release();
	Mango::RescourcePool<Mango::CubeTexture>::Get()->Release();
}

void MangoApp::OnTick()
{
	// handle movement
	{
		static constexpr float MOVE_SPEED = 10.f;

		static const glm::vec3 up_dir = { 0.f, 1.f, 0.f };
		const auto forward_dir = Mango::Maths::AngleVector({ m_camera.GetViewangle().x, 0.f, 0.f });
		const auto right_dir = glm::normalize(glm::cross(forward_dir, up_dir));

		if (m_input_handler.GetKeyState('W'))
			m_camera.Move(forward_dir * m_interval_per_tick * MOVE_SPEED);
		if (m_input_handler.GetKeyState('S'))
			m_camera.Move(-forward_dir * m_interval_per_tick * MOVE_SPEED);
		if (m_input_handler.GetKeyState('D'))
			m_camera.Move(right_dir * m_interval_per_tick * MOVE_SPEED);
		if (m_input_handler.GetKeyState('A'))
			m_camera.Move(-right_dir * m_interval_per_tick * MOVE_SPEED);
		if (m_input_handler.GetKeyState(GLFW_KEY_SPACE))
			m_camera.Move(up_dir * m_interval_per_tick * MOVE_SPEED);
		if (m_input_handler.GetKeyState(GLFW_KEY_LEFT_SHIFT))
			m_camera.Move(-up_dir * m_interval_per_tick * MOVE_SPEED);
	}
}
void MangoApp::OnFrame(float frame_time, float lerptime)
{
	// mouse input
	{
		static constexpr float CAMERA_SPEED = 0.005f;
		const auto screen_center = m_mango_core.GetWindowSize() / 2;

		static bool toggle = false;
		if (toggle)
		{
			const auto offset = glm::vec2(m_mango_core.GetMousePosition().x - screen_center.x, m_mango_core.GetMousePosition().y - screen_center.y);
			const auto new_viewangle = m_camera.GetViewangle() + glm::vec3(offset[0], -offset[1], 0.f) * CAMERA_SPEED;
			m_camera.SetViewangle(new_viewangle);
			m_mango_core.SetMousePosition(screen_center);
		}

		// toggle
		if (m_input_handler.GetButtonState(2) == Mango::INPUT_STATE::INPUT_STATE_PRESS)
		{
			toggle = !toggle;
			if (toggle)
				m_mango_core.SetMousePosition(screen_center);
		}
	}

	auto& renderer_2d = m_mango_core.GetRenderer2D();
	auto& renderer_3d = m_mango_core.GetRenderer3D();

	m_framebuffer.Bind();
	m_mango_core.Clear({ 0.f, 0.f, 0.f });

	// 3d rendering
	{
		renderer_3d.Start();

		auto cube_shader = Mango::RescourcePool<Mango::Shader>::Get()->GetRes("cube_shader");
		auto cube_model = Mango::RescourcePool<Mango::Model>::Get()->GetRes("cube");

		// model
		cube_model->GetVAO().Bind();

		m_chunk.GetVBO()->Bind();
		Mango::VertexArray::EnableAttributeInt(1, 1, GL_UNSIGNED_BYTE, 1, 0, 1);
		Mango::VertexBuffer::Unbind();

		// shader
		cube_shader->Bind();

		cube_shader->SetUniformMat4("u_projection_matrix", renderer_3d.GetProjMatrix());
		cube_shader->SetUniformMat4("u_view_matrix", m_camera.GetViewMatrix());
		cube_shader->SetUniformMat4("u_model_matrix", Mango::Maths::CreateModelMatrix({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }));

		int values[16];
		for (int i = 0; i < 16; i++)
			values[i] = i;
		glUniform1iv(cube_shader->GetUniformLoc("u_textures[]"), 16, values);

		for (size_t i = 0; i < m_block_names.size() - 1; i++)
			Mango::RescourcePool<Mango::CubeTexture>::Get()->GetRes(m_block_names[i + 1])->Bind(i);

		glDrawElementsInstanced(cube_model->GetMode(), cube_model->GetIBO().GetCount(), cube_model->GetIBO().GetType(), nullptr, Chunk::WIDTH * Chunk::HEIGHT * Chunk::DEPTH);

		Mango::Shader::Unbind();
		Mango::CubeTexture::Unbind();
		Mango::VertexArray::Unbind();
		renderer_3d.End();
	}

	// 2d rendering
	{
		renderer_2d.Start();

		Mango::RescourcePool<Mango::Texture>::Get()->GetRes("mango")->Bind();
		renderer_2d.RenderTexturedQuad(m_mango_core.GetMousePosition() - 10.f, m_mango_core.GetMousePosition() + 10.f);

		renderer_2d.End();
	}

	// render final mango
	{
		Mango::Framebuffer::Unbind();
		m_mango_core.Clear({ 0.f, 0.f, 0.f });

		renderer_2d.Start();
		m_framebuffer.GetTexture().Bind();
		renderer_2d.RenderTexturedQuad({ 0, 0 }, m_mango_core.GetWindowSize());
		renderer_2d.End();
	}

	// imgui
	{
		ImGui::Begin("Mango");

		ImGui::Text("FPS: ");
		ImGui::SameLine(0.f, 0.f);
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "%.0f", 1.f / m_mango_core.GetFrameTime());

		if (static bool c = true; ImGui::Checkbox("Vertical Sync", &c))
			m_mango_core.SetVerticalSync(c);

		ImGui::End();
	}
}