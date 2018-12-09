#pragma once

#include "../../common.h"

#include "../wrappers/shader.h"
#include "../wrappers/vertex_array.h"
#include "../wrappers/buffer_object.h"
#include "../wrappers/texture.h"

#include "../model.h"




namespace Mango
{
	class MangoCore;

	class Renderer2D : private Utility
	{
	public:
		Renderer2D() = default;
		~Renderer2D() { Release(); UnregisterUtil(); }

		bool Setup(MangoCore* mango);
		void Release() override;
		void Resize(int width, int height);

		void Start();
		void End();

		void RenderQuad(glm::ivec2 pos1, glm::ivec2 pos2, glm::vec3 color);
		void RenderTexturedQuad(glm::ivec2 pos1, glm::ivec2 pos2);

	private:
		bool is_init = false;
		glm::mat4 m_proj_matrix;
		MangoCore* m_mango = nullptr;

		Shader m_flat_shader,
			m_texture_shader;
		Model m_quad_model;
	};
} // namespace Mango