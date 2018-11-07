#include "texture.h"


namespace Mango
{
	bool Texture::Setup(const std::string& file_path, float mip_map_lod)
	{
		int num_components;
		stbi_set_flip_vertically_on_load(1);
		const auto buffer = stbi_load(file_path.c_str(), &m_size.x, &m_size.y, &num_components, 4);
		if (!buffer)
		{
			DBG_ERROR("Failed to load texture: \"%s\"", file_path.c_str());
			return false;
		}

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

		// mipmap stuffs
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, mip_map_lod);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (buffer)
			stbi_image_free(buffer);

		return true;
	}
	void Texture::Release()
	{
		if (!IsUtilRegistered())
			return;
		UnregisterUtil();

		if (m_texture == 0xFFFFFFFF)
			return;

		glDeleteTextures(1, &m_texture);
	}

	void Texture::Bind(unsigned int slot) const
	{
		// minimum 48 slots in opengl 3.3
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
} // namespace Mango