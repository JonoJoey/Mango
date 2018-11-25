#include "texture.h"


namespace Mango
{
	bool Texture::Setup(const std::string& file_path, bool srgb, bool alpha_channel, bool linear, float mip_map_lod)
	{
		int num_components; glm::ivec2 size;
		stbi_set_flip_vertically_on_load(1);
		const auto buffer = stbi_load(file_path.c_str(), &size.x, &size.y, &num_components, alpha_channel ? 4 : 3);
		if (!buffer)
		{
			DBG_ERROR("Failed to load texture: \"%s\"", file_path.c_str());
			return false;
		}

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, alpha_channel ? (srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8) : (srgb ? GL_SRGB8 : GL_RGB8), size.x, size.y, 0, alpha_channel ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, buffer);

		// mipmap stuffs
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, mip_map_lod);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);

		// repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(buffer);
		return true;
	}
	void Texture::Setup(unsigned int texture)
	{
		m_texture = texture;
	}
	void Texture::Release()
	{
		if (m_texture == 0xFFFFFFFF)
			return;

		glDeleteTextures(1, &m_texture);
		m_texture = 0xFFFFFFFF;
	}
	void Texture::Bind(unsigned int slot) const
	{
		// minimum 48 slots in opengl 3.3
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
	void Texture::Unbind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	bool CubeTexture::Setup(const std::array<std::string, 6> file_paths, bool srgb, bool alpha_channel, bool linear)
	{
		glGenTextures(1, &m_texture);
		Bind();

		stbi_set_flip_vertically_on_load(0);
		int num_components; glm::ivec2 size;
		for (int i = 0; i < 6; i++)
		{
			const auto buffer = stbi_load(file_paths[i].c_str(), &size.x, &size.y, &num_components, alpha_channel ? 4 : 3);
			if (!buffer)
			{
				DBG_ERROR("Failed to load texture: \"%s\"", file_paths[i].c_str());
				Release();
				return false;
			}
			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, alpha_channel ? (srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8) : (srgb ? GL_SRGB8 : GL_RGB8),
				size.x, size.y, 0, alpha_channel ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, buffer);
			stbi_image_free(buffer);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		Unbind();

		return true;
	}
	void CubeTexture::Release()
	{
		if (m_texture == 0xFFFFFFFF)
			return;

		glDeleteTextures(1, &m_texture);
		m_texture = 0xFFFFFFFF;
	}
	void CubeTexture::Bind(unsigned int slot) const
	{
		// minimum 48 slots in opengl 3.3
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	}
	void CubeTexture::Unbind(unsigned int slot)
	{
		// minimum 48 slots in opengl 3.3
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}


	bool TextureArray::Setup(std::vector<std::string> file_paths, glm::ivec2 size, bool srgb, bool alpha_channel, bool linear)
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);

		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, alpha_channel ? (srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8) : (srgb ? GL_SRGB8 : GL_RGB8),
			size.x, size.y, file_paths.size(), 0, alpha_channel ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

		for (size_t i = 0; i < file_paths.size(); i++)
		{
			int num_components; glm::ivec2 s;
			stbi_set_flip_vertically_on_load(1);
			const auto buffer = stbi_load(file_paths[i].c_str(), &s.x, &s.y, &num_components, alpha_channel ? 4 : 3);
			if (!buffer)
			{
				DBG_ERROR("Failed to load texture");
				Release();
				return false;
			}

			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, s.x, s.y, 1, 
				alpha_channel ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, buffer);

			stbi_image_free(buffer);
		}

		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		return true;
	}
	void TextureArray::Release()
	{
		if (m_texture == 0xFFFFFFFF)
			return;

		glDeleteTextures(1, &m_texture);
		m_texture = 0xFFFFFFFF;
	}
	void TextureArray::Bind(unsigned int slot) const
	{
		// minimum 48 slots in opengl 3.3
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
	}
	void TextureArray::Unbind(unsigned int slot)
	{
		// minimum 48 slots in opengl 3.3
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}
} // namespace Mango