#pragma once

#include "../../common.h"
#include "../wrappers/texture.h"
#include "../wrappers/shader.h"
#include "../model.h"

#include <unordered_map>


namespace Mango
{
	typedef uint32_t FONT_HANDLE;
	static constexpr FONT_HANDLE INVALID_FONT_HANDLE(0);

	class TextRenderer
	{
	public:
		bool Setup();
		void Release();

		FONT_HANDLE NewFont(std::string name, std::string file_path, int font_height);
		bool RenderText(FONT_HANDLE font, const glm::mat4& projection_matrix, const glm::ivec2& position, const glm::vec4& color, const char* text);

	private:
		struct Character
		{
			glm::ivec2 m_size;
			glm::ivec2 m_bearing;
			unsigned int m_advance;
			bool m_loaded;
		};
		struct FontInfo
		{
			std::string m_name,
				m_file_path;
			TextureArray m_texture;
			std::array<Character, 256> m_characters;
		};

	private:
		Shader m_shader;
		Model m_quad_model;
		FT_Library m_freetype_library;
		std::unordered_map<FONT_HANDLE, FontInfo> m_fonts;
	};
} // namespace Mango