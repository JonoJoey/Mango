#include "mango.h"


namespace 
{
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

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

	bool MangoCore::m_is_init = false;

	bool MangoCore::Setup(const std::string& window_name, const glm::ivec2& window_size)
	{
		if (m_is_init)
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
		glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

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
		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
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

		m_is_init = true;
		return true;
	}
	void MangoCore::Release()
	{
		if (!m_is_init)
			return;

		Utility::Cleanup();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_window);
		glfwTerminate();

		m_is_init = false;
	}

	bool MangoCore::NextFrame(const glm::vec3& clear_color)
	{
		if (glfwWindowShouldClose(m_window))
			return false;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.f);

		// imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

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

	void MangoCore::RegisterInput(IInput* input)
	{

	}
} // namespace Mango