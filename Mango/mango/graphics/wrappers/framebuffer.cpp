#include "framebuffer.h"


namespace Mango
{
	bool Framebuffer::Setup(const glm::ivec2& size)
	{
		m_size = size;

		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		// setup the texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_texture.Setup(texture);

		// setup the render buffer
		glGenRenderbuffers(1, &m_render_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// check if created
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			DBG_ERROR("Failed to create framebuffer");
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}
	void Framebuffer::Release()
	{
		if (!IsUtilRegistered())
			return;
		UnregisterUtil();

		if (m_framebuffer == 0xFFFFFFFF)
			return;

		glDeleteFramebuffers(1, &m_framebuffer);
		m_framebuffer = 0xFFFFFFFF;

		glDeleteRenderbuffers(1, &m_render_buffer);
		m_render_buffer = 0xFFFFFFFF;

		m_texture.Release();
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	}
	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
} // namespace Mango