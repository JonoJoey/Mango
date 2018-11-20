#pragma once

#include "common.h"

#include "graphics/wrappers/shader.h"
#include "graphics/wrappers/buffer_object.h"
#include "graphics/wrappers/vertex_array.h"
#include "graphics/wrappers/framebuffer.h"

#include "graphics/model.h"

#include "graphics/2D/renderer_2d.h"
#include "graphics/3D/renderer_3d.h"

#include "misc/event_handler.h"
#include "misc/input_handler.h"
#include "misc/file_parser.h"
#include "misc/rescource_pool.h"


namespace Mango
{
	class MangoCore
	{
	public:
		MangoCore() = default;
		~MangoCore() { Release(); }

		bool Setup(const std::string& window_name, const glm::ivec2& window_size);
		void Release();

		bool NextFrame();
		void EndFrame();
		void Clear(glm::vec3 color);

		void SetVerticalSync(bool on);
		void SetMousePosition(glm::vec2 position);

		glm::ivec2 GetWindowSize() const { int w, h; glfwGetWindowSize(m_window, &w, &h); return { w, h }; }
		float GetAspectRatio() const { int w, h; glfwGetWindowSize(m_window, &w, &h); return float(w) / float(h); }
		float GetFrameTime() const { return m_frame_time; }
		glm::vec2 GetMousePosition() const { double xpos, ypos; glfwGetCursorPos(m_window, &xpos, &ypos); return { float(xpos), float(ypos) }; }

		bool RegisterEventHandler(IEventHandler* event_handler);
		void UnregisterEventHandler(IEventHandler* event_handler);
		static std::deque<IEventHandler*>& GetEventInterfaces() { return event_interfaces; }

		Renderer2D& GetRenderer2D() { return m_renderer_2d; }
		Renderer3D& GetRenderer3D() { return m_renderer_3d; }

	private:
		static bool is_init;
		static std::deque<IEventHandler*> event_interfaces;

		GLFWwindow* m_window = nullptr;
		Renderer2D m_renderer_2d;
		Renderer3D m_renderer_3d;
		float m_frame_time = 1.f;
	};
} // namespace Mango