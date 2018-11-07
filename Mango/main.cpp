#include <Windows.h>

#include "mango/mango.h"


int main()
{
	Mango::MangoCore mango;
	if (!mango.Setup("Mango", { 800, 600 }))
	{
		DBG_ERROR("Failed to setup mango");
		system("pause");
		return EXIT_FAILURE;
	}

	Mango::TrianglePos2D triangle_pos;
	triangle_pos.m_vertex_1 = { 400, 100 };
	triangle_pos.m_vertex_2 = { 100, 500 };
	triangle_pos.m_vertex_3 = { 700, 500 };

	Mango::TrianglePos2D triangle_pos_2;
	triangle_pos_2.m_vertex_1 = { 500, 100 };
	triangle_pos_2.m_vertex_2 = { 200, 500 };
	triangle_pos_2.m_vertex_3 = { 800, 500 };

	Mango::TriangleColor2D triangle_col;
	triangle_col.m_color_1 = { 255, 0, 0, 255 };
	triangle_col.m_color_2 = { 0, 255, 0, 255 };
	triangle_col.m_color_3 = { 0, 0, 255, 255 };

	float vertices[] = 
	{
		400.f, 100.f, 
		100.f, 500.f,
		700.f, 500.f
	};
	unsigned int indices[] =
	{
		0, 1, 2
	};

	//Mango::Shader shader(Mango::Shader::ReadFile("res/shaders/basic_vs.glsl"), Mango::Shader::ReadFile("res/shaders/basic_fs.glsl"));
	//shader.Bind();
	//
	//Mango::VertexArray vao;
	//vao.Setup();
	//vao.Bind();
	//
	//Mango::IndexBuffer ibo(3, sizeof(unsigned int), indices, GL_STATIC_DRAW);
	//ibo.Bind();
	//
	//Mango::VertexBuffer position_vbo(3, sizeof(glm::i16vec2), &triangle_pos, GL_STATIC_DRAW);
	//position_vbo.Bind();
	//Mango::VertexArray::EnableAttribute(0, 2, GL_SHORT, GL_FALSE, 0, 0);
	//
	//Mango::VertexBuffer color_vbo(3, sizeof(glm::u8vec4), &triangle_col, GL_STATIC_DRAW);
	//color_vbo.Bind();
	//Mango::VertexArray::EnableAttribute(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
	
	while (mango.NextFrame({ 0.f, 1.f, 1.f }))
	{
		ImGui::Begin("Mango");
		ImGui::Text("Mango?");
		ImGui::Text("Mango.");
		ImGui::End();

		mango.GetRenderer2D().Start();
		mango.GetRenderer2D().RenderTriangle(triangle_pos, triangle_col);
		mango.GetRenderer2D().RenderTriangle(triangle_pos_2, triangle_col);
		mango.GetRenderer2D().End();
		// 2d rendering
		//mango.GetRenderer2D().Start();
		//shader.Bind();
		//
		//Mango::Shader::SetUniformMat4(shader.GetUniformLoc("u_ProjectionMatrix"), glm::ortho(0.f, 800.f, 600.f, 0.f));
		//Mango::Shader::SetUniformMat4(shader.GetUniformLoc("u_ViewMatrix"), glm::mat4(1.f));
		//
		//glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
		//
		//shader.Unbind();
		//mango.GetRenderer2D().End();

		mango.EndFrame();
	}

	mango.Release();
	return EXIT_SUCCESS;
}