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

	float vertices[] = {
		400.f, 500.f, 1.f,
		100.f, 100.f, 1.f,
		700.f, 100.f, 1.f
	};
	float colors[] = {
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	Mango::Model cube_model;
	cube_model.Setup(GL_TRIANGLES, 3, GL_UNSIGNED_INT, indices);
	cube_model.GetVAO().Bind();

	auto vbo = &cube_model.AddVBO();
	vbo->Setup(sizeof(float) * 3 * 3, vertices);
	vbo->Bind();
	Mango::VertexArray::EnableAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	vbo = &cube_model.AddVBO();
	vbo->Setup(sizeof(float) * 3 * 4, colors);
	vbo->Bind();
	Mango::VertexArray::EnableAttribute(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	Mango::VertexArray::Unbind();

	Mango::Shader shader(Mango::Shader::ReadFile("res/shaders/basic_vs.glsl"),
		Mango::Shader::ReadFile("res/shaders/basic_fs.glsl"));
	shader.Bind();
	Mango::Shader::SetUniformMat4(shader.GetUniformLoc("u_projection_matrix"), glm::ortho(0.f, 800.f, 0.f, 600.f));
	Mango::Shader::SetUniformMat4(shader.GetUniformLoc("u_view_matrix"), glm::mat4(1.f));
	Mango::Shader::SetUniformMat4(shader.GetUniformLoc("u_model_matrix"), glm::mat4(1.f));
	Mango::Shader::Unbind();

	Mango::Texture texture("res/textures/mango.jpg");
	
	while (mango.NextFrame({ 0.f, 1.f, 1.f }))
	{
		ImGui::Begin("Mango");
		ImGui::Text("Mango?");
		ImGui::Text("Mango.");
		ImGui::End();


		cube_model.GetVAO().Bind();
		shader.Bind();

		glDrawElements(cube_model.GetMode(), cube_model.GetIBO().GetCount(), cube_model.GetIBO().GetType(), nullptr);

		Mango::Shader::Unbind();
		Mango::VertexArray::Unbind();

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