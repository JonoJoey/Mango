#pragma once

#include "../../common.h"


namespace Mango
{
	class Texture : private Utility
	{
	public:
		Texture() = default;
		Texture(const std::string& file_path, bool srgb = false, bool alpha_channel = false, bool linear = false, float mip_map_lod = 0.f) { ASSERT(Setup(file_path, srgb, alpha_channel, linear, mip_map_lod)); }
		Texture(unsigned int texture) { Setup(texture); }
		~Texture() { Release(); UnregisterUtil(); }

		bool Setup(const std::string& file_path, bool srgb = false, bool alpha_channel = false, bool linear = false, float mip_map_lod = 0.f);
		void Setup(unsigned int texture);
		void Release() override;

		void Bind(unsigned int slot = 0) const;
		static void Unbind(unsigned int slot = 0);

		unsigned int GetTexture() const { return m_texture; }

	private:
		unsigned int m_texture = 0xFFFFFFFF;
	};

	class CubeTexture : private Utility
	{
	public:
		CubeTexture() = default;
		CubeTexture(const std::array<std::string, 6> file_paths, bool srgb = false, bool alpha_channel = false, bool linear = true)
		{
			ASSERT(Setup(file_paths, srgb, alpha_channel, linear));
		}
		~CubeTexture() { Release(); UnregisterUtil(); }

		// right, left, top, bottom, back, front
		bool Setup(const std::array<std::string, 6> file_paths, bool srgb = false, bool alpha_channel = false, bool linear = true);
		void Release() override;

		void Bind(unsigned int slot = 0) const;
		static void Unbind(unsigned int slot = 0);

	private:
		unsigned int m_texture = 0xFFFFFFFF;
	};

	class TextureArray : private Utility
	{
	public:
		TextureArray() = default;
		TextureArray(std::vector<std::string> file_paths, glm::ivec2 size, bool srgb = false, bool alpha_channel = false, bool linear = true, float mip_map_lod = 0.f)
		{
			ASSERT(Setup(file_paths, size, srgb, alpha_channel, linear, mip_map_lod));
		}
		~TextureArray() { Release(); UnregisterUtil(); }

		bool Setup(std::vector<std::string> file_paths, glm::ivec2 size, bool srgb = false, bool alpha_channel = false, bool linear = true, float mip_map_lod = 0.f);
		void Release() override;

		void Bind(unsigned int slot = 0) const;
		static void Unbind(unsigned int slot = 0);

	private:
		unsigned int m_texture = 0xFFFFFFFF;
	};
} // namespace Mango