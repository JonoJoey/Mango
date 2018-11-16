#pragma once

#include "../../common.h"


namespace Mango
{
	class Texture : private Utility
	{
	public:
		Texture() = default;
		Texture(const std::string& file_path, bool alpha_channel = false, float mip_map_lod = 0.f) { ASSERT(Setup(file_path, alpha_channel, mip_map_lod)); }
		Texture(unsigned int texture) { Setup(texture); }
		~Texture() { Release(); }

		bool Setup(const std::string& file_path, bool alpha_channel = false, float mip_map_lod = 0.f);
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
		~CubeTexture() { Release(); }

		bool Setup(const std::array<std::string, 6> file_paths, bool alpha_channel = false);
		void Release() override;

		void Bind(unsigned int slot = 0) const;
		static void Unbind(unsigned int slot = 0);

	private:
		unsigned int m_texture = 0xFFFFFFFF;
	};
} // namespace Mango