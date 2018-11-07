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

		DBG_LOG("type: 0x%x, severity = 0x%x, message = %s", type, severity, message);
	}
} // namespace

namespace Mango
{
	bool Mango::Setup(const std::string& window_name, const glm::ivec2& window_size)
	{
		ASSERT(!m_is_init);

		// init glfw
		if (!glfwInit())
		{
			DBG_LOG("glfwInit() failure");
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// create glfw window
		m_window = glfwCreateWindow(window_size[0], window_size[1], window_name.c_str(), nullptr, nullptr);
		if (!m_window)
		{
			DBG_LOG("glfwCreateWindow() failure");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);
		glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

		// init glew
		if (const auto error = glewInit(); error != GLEW_OK)
		{
			DBG_LOG("glewInit() failure %s", glewGetErrorString(error));
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

		m_is_init = true;
		return true;
	}
	void Mango::Release()
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

	bool Mango::NextFrame(const glm::vec3& clear_color)
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
	void Mango::EndFrame()
	{
		// imgui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
} // namespace Mango