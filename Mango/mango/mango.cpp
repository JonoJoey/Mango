#include "mango.h"


namespace 
{
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		for (auto input : Mango::MangoCore::GetEventInterfaces())
			input->OnWindowResize(width, height);
	}

	// input
	void MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		for (auto input : Mango::MangoCore::GetEventInterfaces())
			input->OnMouseMove(float(xpos), float(ypos));
	}
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

		for (auto input : Mango::MangoCore::GetEventInterfaces())
		{
			switch (action)
			{
			case GLFW_PRESS:
			case GLFW_REPEAT:
				input->OnMouseButtonPress(button, action == GLFW_REPEAT);
				break;
			case GLFW_RELEASE:
				input->OnMouseButtonRelease(button);
				break;
			}
		}
	}
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	}
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

		auto c_str = glfwGetKeyName(key, scancode);
		std::string key_name = c_str ? c_str : "";

		// GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
		for (auto input : Mango::MangoCore::GetEventInterfaces())
		{
			switch (action)
			{
			case GLFW_PRESS:
			case GLFW_REPEAT:
				input->OnKeyPress(key, key_name, action == GLFW_REPEAT);
				break;
			case GLFW_RELEASE:
				input->OnKeyRelease(key, key_name);
				break;
			}
		}
	}
	void CharCallback(GLFWwindow* window, unsigned int codepoint)
	{
		ImGui_ImplGlfw_CharCallback(window, codepoint);
	}

	// opengl messages
	void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, 
		GLsizei length, const GLchar* message, const void* user_param)
	{
		if (type != GL_DEBUG_TYPE_ERROR)
			return;

		if (type == GL_DEBUG_TYPE_ERROR)
			DBG_ERROR("severity = 0x%x, message = %s", severity, message);
		else
			DBG_LOG("Message - type: 0x%x, severity = 0x%x, message = %s", type, severity, message);
	}
} // namespace

namespace Mango
{
	bool MangoCore::is_init = false;
	std::deque<IEventHandler*> MangoCore::event_interfaces;

	bool MangoCore::Setup(const std::string& window_name, const glm::ivec2& window_size)
	{
		if (is_init)
		{
			DBG_ERROR("Cannot have more than 2 MangoCores running at the same time!");
			return false;
		}

		// init glfw
		if (!glfwInit())
		{
			DBG_ERROR("glfwInit() failure");
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// create glfw window
		m_window = glfwCreateWindow(window_size[0], window_size[1], window_name.c_str(), nullptr, nullptr);
		if (!m_window)
		{
			DBG_ERROR("glfwCreateWindow() failure");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);

		// callbacks
		glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
		glfwSetCursorPosCallback(m_window, MousePositionCallback);
		glfwSetKeyCallback(m_window, KeyCallback);
		glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
		glfwSetScrollCallback(m_window, ScrollCallback);
		glfwSetCharCallback(m_window, CharCallback);

		// init glew
		if (const auto error = glewInit(); error != GLEW_OK)
		{
			DBG_ERROR("glewInit() failure %s", glewGetErrorString(error));
			glfwDestroyWindow(m_window);
			glfwTerminate();
			return false;
		}

		// Setup Dear ImGui binding
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(m_window, false);
		ImGui_ImplOpenGL3_Init();
		ImGui::StyleColorsDark();

		// blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// depth test
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// message callback
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		// setup 2D renderer
		if (!m_renderer_2d.Setup(this))
		{
			DBG_ERROR("Failed to setup 2D renderer");
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			glfwDestroyWindow(m_window);
			glfwTerminate();
			return false;
		}

		// setup 3D renderer
		if (!m_renderer_3d.Setup(this))
		{
			DBG_ERROR("Failed to setup 3D renderer");
			m_renderer_2d.Release();

			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			glfwDestroyWindow(m_window);
			glfwTerminate();
			return false;
		}

		is_init = true;

		return true;
	}
	void MangoCore::Release()
	{
		if (!is_init)
			return;

		is_init = false;
		event_interfaces.clear();

		Utility::Cleanup();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	bool MangoCore::NextFrame(const glm::vec3& clear_color)
	{
		if (glfwWindowShouldClose(m_window))
			return false;

		glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static double last_time = 0.0;
		const double time = glfwGetTime();
		m_frame_time = float(time - last_time);
		last_time = time;

		return true;
	}
	void MangoCore::EndFrame()
	{
		// imgui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	bool MangoCore::RegisterEventHandler(IEventHandler* event_handler)
	{
		ASSERT(event_handler);
		
		for (auto i : event_interfaces)
		{
			if (i == event_handler)
			{
				DBG_ERROR("Input interface already registered");
				return false;
			}
		}

		event_interfaces.push_back(event_handler);
		return true;
	}
	void MangoCore::UnregisterEventHandler(IEventHandler* event_handler)
	{
		for (size_t i = 0; i < event_interfaces.size(); i++)
		{
			if (event_interfaces[i] == event_handler)
			{
				event_interfaces.erase(event_interfaces.begin() + i);
				return;
			}
		}
	}

	void MangoCore::SetMousePosition(glm::vec2 position)
	{
		glfwSetCursorPos(m_window, double(position.x), double(position.y));
	}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <GL/wglew.h>
	void MangoCore::SetVerticalSync(bool on)
	{
		static auto SwapInterval = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		SwapInterval(on ? 1 : 0);
	}
#else
	void MangoCore::SetVerticalSync(bool on) {}
#endif
} // namespace Mango