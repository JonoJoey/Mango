#pragma once

#include "../../common.h"


namespace Mango
{
	class Texture : private Utility
	{
	public:
		Texture() = default;
		Texture(const std::string& file_path, float mip_map_lod = 0.f) { ASSERT(Setup(file_path, mip_map_lod)); }
		Texture(unsigned int texture) { Setup(texture); }
		~Texture() { Release(); }

		bool Setup(const std::string& file_path, float mip_map_lod = 0.f);
		void Setup(unsigned int texture);
		void Release() override;

		void Bind(unsigned int slot = 0) const;
		static void Unbind(unsigned int slot = 0);

		unsigned int GetTexture() const { return m_texture; }
		glm::ivec2 GetSize() const { return m_size; }

	private:
		unsigned int m_texture = 0xFFFFFFFF;
		glm::ivec2 m_size;
	};
} // namespace Mango