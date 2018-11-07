#include <Windows.h>

#include "mango/mango.h"


void CheckErrors()
{
	unsigned int error;
	while ((error = glGetError()) != GL_NO_ERROR)
	{

	}
}
int main()
{
	Mango::Mango mango;
	if (!mango.Setup("Mango", { 800, 600 }))
	{
		DBG_LOG("Failed to setup mango");
		system("pause");
		return EXIT_FAILURE;
	}

	if (!mango.GetRenderer2D().Setup())
	{
		DBG_LOG("Failed to setup 2D renderer");
		system("pause");
		return EXIT_FAILURE;
	}

	float vertices[] = 
	{
		400.f, 100.f, 
		100.f, 500.f,
		700.f, 500.f
	};

	Mango::Shader shader(Mango::Shader::ReadFile("res/shaders/basic_vs.glsl"), Mango::Shader::ReadFile("res/shaders/basic_fs.glsl"));
	shader.Bind();



	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

	while (mango.NextFrame({ 0.f, 1.f, 1.f }))
	{
		ImGui::Begin("Mango");
		ImGui::Text("Mango?");
		ImGui::Text("Mango.");
		ImGui::End();

		// 2d rendering
		mango.GetRenderer2D().Start();
		shader.Bind();

		Mango::Shader::SetUniformMat4(shader.GetUniformLoc("u_ProjectionMatrix"), glm::ortho(0.f, 800.f, 600.f, 0.f));
		Mango::Shader::SetUniformMat4(shader.GetUniformLoc("u_ViewMatrix"), glm::mat4(1.f));
		Mango::Shader::SetUniformMat4(shader.GetUniformLoc("u_ModelMatrix"), glm::mat4(1.f));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		shader.Unbind();
		mango.GetRenderer2D().End();

		mango.EndFrame();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	mango.Release();
	system("pause");
	return EXIT_SUCCESS;
}