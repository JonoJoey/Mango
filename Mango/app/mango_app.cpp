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

	// renderers
	auto& renderer_2d = m_mango_core.GetRenderer2D();
	auto& renderer_3d = m_mango_core.GetRenderer3D();

	// framebuffer
	m_framebuffer.Setup(m_mango_core.GetWindowSize());

	Mango::Texture mango("res/textures/mango.png", true, true);

	// loooooooooooop
	while (m_mango_core.NextFrame())
	{
		m_framebuffer.Bind();
		m_mango_core.Clear({ 0.f, 0.f, 1.f });

		m_ecs_core.OnFrame();

		// 3d rendering
		{
			renderer_3d.Start();

			renderer_3d.End();
		}

		// 2d rendering
		{
			renderer_2d.Start();

			mango.Bind();

			renderer_2d.RenderQuad({ 0, 0 }, { 600, 600 }, { 1.f, 0.f, 0.f });
			for (int x = 0; x < 50; x++)
			{
				for (int y = 0; y < 50; y++)
				{
					renderer_2d.RenderTexturedQuad({ x * 20, y * 20 }, { 20 + x * 20, 20 + y * 20 });
				}
			}

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
			ImGui::End();
		}

		m_mango_core.EndFrame();
	}

	// release
	m_ecs_core.Release();
	m_mango_core.Release();
}