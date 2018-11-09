#pragma once

#include "common.h"

#include "misc/utility.h"

#include "graphics/wrappers/shader.h"
#include "graphics/wrappers/buffer_object.h"
#include "graphics/wrappers/vertex_array.h"

#include "graphics/model.h"

#include "graphics/renderer_2d.h"
#include "graphics/renderer_3d.h"

namespace Mango
{
	class MangoCore
	{
	public:
		MangoCore() = default;
		~MangoCore() { Release(); }

		bool Setup(const std::string& window_name, const glm::ivec2& window_size);
		void Release();

		bool NextFrame(const glm::vec3& clear_color);
		void EndFrame();

		glm::ivec2 GetWindowSize() const { int w, h; glfwGetWindowSize(m_window, &w, &h); return { w, h }; }

		Renderer2D& GetRenderer2D() { return m_renderer_2d; }
		Renderer3D& GetRenderer3D() { return m_renderer_3d; }

	private:
		bool m_is_init = false;
		GLFWwindow* m_window = nullptr;

		Renderer2D m_renderer_2d;
		Renderer3D m_renderer_3d;
	};
} // namespace Mango