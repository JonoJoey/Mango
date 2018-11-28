#pragma once

#include "../../common.h"

#include "texture.h"


namespace Mango
{
	class Framebuffer : private Utility
	{
	public:
		Framebuffer() = default;
		Framebuffer(const glm::ivec2& size) { ASSERT(Setup(size)); }
		~Framebuffer() { Release(); UnregisterUtil(); }

		bool Setup(const glm::ivec2& size);
		void Release() override;

		void Bind() const;
		static void Unbind();

		Texture& GetTexture() { return m_texture; }

	private:
		unsigned int m_framebuffer = 0xFFFFFFFF,
			m_render_buffer = 0xFFFFFFFF;
		Texture m_texture;
		glm::ivec2 m_size;
	};
} // namespace Mango