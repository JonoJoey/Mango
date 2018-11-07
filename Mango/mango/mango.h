#pragma once

#include "common.h"

#include "misc/utility.h"

#include "graphics/renderer_2d.h"
#include "graphics/wrappers/shader.h"


namespace Mango
{
	class Mango
	{
	public:
		Mango() = default;
		~Mango() { Release(); }

		bool Setup(const std::string& window_name, const glm::ivec2& window_size);
		void Release();

		bool NextFrame(const glm::vec3& clear_color);
		void EndFrame();

		Renderer2D& GetRenderer2D() { return m_renderer_2d; }

	private:
		bool m_is_init = false;
		GLFWwindow* m_window = nullptr;

		Renderer2D m_renderer_2d;
	};
} // namespace Mango