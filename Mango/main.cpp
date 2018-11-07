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

	Mango::Texture texture;
	ASSERT(texture.Setup("res/textures/mango.jpg"));
	
	while (mango.NextFrame({ 0.f, 1.f, 1.f }))
	{
		ImGui::Begin("Mango");
		ImGui::Text("Mango?");
		ImGui::Text("Mango.");
		ImGui::End();

		mango.GetRenderer2D().Start();
		mango.GetRenderer2D().End();


		mango.GetRenderer3D().Start();

		mango.GetRenderer3D().End();

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