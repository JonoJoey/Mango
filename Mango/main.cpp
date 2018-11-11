#include <Windows.h>

#include "mango/mango.h"


bool LoadModel(Mango::Model& model, const std::string& file_path)
{
	Mango::WavefrontFormat format;
	if (!Mango::LoadWavefront(file_path, format))
		return false;

	model.Setup(GL_TRIANGLES, format.m_indices.size() * 3, GL_UNSIGNED_INT, format.m_indices.data());
	model.GetVAO().Bind();

	auto vbo = &model.AddVBO();
	vbo->Setup(format.m_positions.size() * 3 * sizeof(float), format.m_positions.data());
	vbo->Bind();
	Mango::VertexArray::EnableAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	vbo = &model.AddVBO();
	vbo->Setup(format.m_tex_coords.size() * 2 * sizeof(float), format.m_tex_coords.data());
	vbo->Bind();
	Mango::VertexArray::EnableAttribute(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	Mango::VertexArray::Unbind();

	return true;
}

void HandleCamera(Mango::Camera3D& camera)
{

}

int main()
{
	Mango::MangoCore mango;
	if (!mango.Setup("Mango", { 800, 600 }))
	{
		DBG_ERROR("Failed to setup mango");
		system("pause");
		return EXIT_FAILURE;
	}

	Mango::Model cube_model;
	if (!LoadModel(cube_model, "res/models/cube.obj"))
	{
		DBG_ERROR("Failed to load model");
		mango.Release();
		system("pause");
		return EXIT_FAILURE;
	}

	Mango::Camera3D camera({ 0.f, 0.f, 1.f });

	Mango::Shader shader(Mango::Shader::ReadFile("res/shaders/basic_vs.glsl"),
		Mango::Shader::ReadFile("res/shaders/basic_fs.glsl"));

	Mango::Texture texture("res/textures/mango.jpg");
	
	while (mango.NextFrame({ 0.f, 1.f, 1.f }))
	{
		ImGui::Begin("Mango");
		ImGui::Text("Mango?");
		ImGui::Text("Mango.");
		ImGui::End();

		cube_model.GetVAO().Bind();
		texture.Bind(0);
		shader.Bind();

		shader.SetUniformMat4("u_projection_matrix", Mango::Maths::CreateProjectionMatrix(60.f, mango.GetAspectRatio(), 0.1f, 300.f));
		shader.SetUniformMat4("u_view_matrix", camera.GetViewMatrix());
		shader.SetUniformMat4("u_model_matrix", Mango::Maths::CreateModelMatrix({ 0.f, 0.f, -1.f },
			{ sin(glfwGetTime()) * glm::pi<float>(), sin(glfwGetTime()) * glm::pi<float>(), sin(glfwGetTime()) * glm::pi<float>() }));

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