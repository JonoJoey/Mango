#include "text_renderer.h"


namespace Mango
{
	bool TextRenderer::Setup()
	{
		if (FT_Init_FreeType(&m_freetype_library))
		{
			DBG_ERROR("Failed to init FreeType library");
			return false;
		}

		const auto app_data = GetAppDataPath();
		if (!m_shader.Setup(Shader::ReadFile(app_data + "/.mango/resource_packs/default/shaders/text_vs.glsl"), 
			Shader::ReadFile(app_data + "/.mango/resource_packs/default/shaders/text_fs.glsl")))
		{
			DBG_ERROR("Failed to setup shader");
			FT_Done_FreeType(m_freetype_library);
			return false;
		}

		static const unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		static const float tex_coords[] =
		{
			0.f, 1.f,
			0.f, 0.f,
			1.f, 0.f,
			1.f, 1.f
		};

		m_quad_model.Setup(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
		m_quad_model.GetVAO().Bind();

		// vertices
		auto* vbo = &m_quad_model.AddVBO();
		vbo->Setup((2 * 4 * sizeof(float)) + (4 * sizeof(int)), nullptr);
		vbo->Bind();
		VertexArray::EnableAttribute(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
		VertexArray::EnableAttributeInt(1, 1, GL_INT, sizeof(int), (2 * 4 * sizeof(float)));

		// tex coords
		vbo = &m_quad_model.AddVBO();
		vbo->Setup(2 * 4 * sizeof(float), tex_coords);
		vbo->Bind();
		VertexArray::EnableAttribute(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		return true;
	}
	void TextRenderer::Release()
	{
		m_shader.Release();
		m_fonts.clear();
		FT_Done_FreeType(m_freetype_library);
	}

	FONT_HANDLE TextRenderer::NewFont(std::string name, std::string file_path, int font_height)
	{
		FONT_HANDLE font_handle = INVALID_FONT_HANDLE + 1;
		while (m_fonts.find(font_handle) != m_fonts.end())
			font_handle++;

		FT_Face face;
		if (FT_New_Face(m_freetype_library, file_path.c_str(), 0, &face))
		{
			DBG_ERROR("Failed to load font - \"%s\"", file_path.c_str());
			return INVALID_FONT_HANDLE;
		}

		auto& font_info = m_fonts[font_handle] = FontInfo();
		font_info.m_name = name;
		font_info.m_file_path = file_path;

		FT_Set_Pixel_Sizes(face, 0, font_height);

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, font_height, 
			font_height, 256, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

		for (int i = 0; i < 256; i++)
		{
			font_info.m_characters[i].m_loaded = false;
			if (FT_Load_Char(face, i, FT_LOAD_RENDER))
				continue;

			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, face->glyph->bitmap.width, face->glyph->bitmap.rows, 1,
				GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

			font_info.m_characters[i].m_size = { face->glyph->bitmap.width, face->glyph->bitmap.rows };
			font_info.m_characters[i].m_bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top };
			font_info.m_characters[i].m_advance = face->glyph->advance.x;
			font_info.m_characters[i].m_loaded = true;
		}

		FT_Done_Face(face);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		font_info.m_texture.Setup(texture);

		return font_handle;
	}
	bool TextRenderer::RenderText(FONT_HANDLE font, const glm::mat4& projection_matrix, const glm::ivec2& position, const glm::vec4& color, const char* text)
	{
		if (font == INVALID_FONT_HANDLE || !text || !text[0])
			return false;

		FontInfo* font_info = nullptr;
		if (auto it = m_fonts.find(font); it == m_fonts.end())
			return false;
		else
			font_info = &it->second;

		m_shader.Bind();	
		m_shader.SetUniformF4("u_color", color);
		m_shader.SetUniformMat4("u_projection_matrix", projection_matrix);

		font_info->m_texture.Bind();
		m_quad_model.GetVAO().Bind();
		m_quad_model.GetVBOs()[0].Bind();

		struct SomethingWeirdBrother
		{
			glm::vec2 m_pos1,
				m_pos2,
				m_pos3,
				m_pos4;
			int banana[4];
		};
		SomethingWeirdBrother vertices;

		int current_x = position[0];
		for (const char* c = text; *c; c++)
		{
			const auto& character = font_info->m_characters[*c];
			if (!character.m_loaded)
				continue;

			float xpos = float(current_x + character.m_bearing.x);
			float ypos = float(position[1] - (character.m_size.y - character.m_bearing.y));

			float w = float(character.m_size.x);
			float h = float(character.m_size.y);

			vertices.banana[0] = vertices.banana[1] = vertices.banana[2] = vertices.banana[3] = int(*c);
			vertices.m_pos1 = { xpos, ypos + h };
			vertices.m_pos2 = { xpos, ypos };
			vertices.m_pos3 = { xpos + w, ypos };
			vertices.m_pos4 = { xpos + w, ypos + h };

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
			glDrawElements(m_quad_model.GetMode(), m_quad_model.GetIBO().GetCount(), m_quad_model.GetIBO().GetType(), nullptr);

			current_x += (character.m_advance >> 6);
		}

		return true;
	}
} // namespace Mango