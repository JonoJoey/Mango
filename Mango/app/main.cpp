#include <Windows.h>

#include "../mango/mango.h"
#include "mango_app.h"


int main()
{
	MangoApp app;
	app.Run();

	return EXIT_SUCCESS;
}
//int DABBB()
//{
//	Mango::MangoCore mango;
//	if (!mango.Setup("Mango", { 800, 600 }))
//	{
//		DBG_ERROR("Failed to setup mango");
//		system("pause");
//		return EXIT_FAILURE;
//	}
//
//	if (!mango.RegisterEventHandler(&g_mango_event_handler))
//	{
//		DBG_ERROR("Failed to setup event handler");
//		system("pause");
//		return EXIT_FAILURE;
//	}
//
//	float quad_vertices[] =
//	{
//		-1.f,  1.f,  0.f, 1.f,
//		-1.f, -1.f,  0.f, 0.f,
//		 1.f, -1.f,  1.f, 0.f,
//
//		-1.f,  1.f,  0.f, 1.f,
//		 1.f, -1.f,  1.f, 0.f,
//		 1.f,  1.f,  1.f, 1.f
//	};
//	unsigned int quad_indices[] =
//	{
//		0, 1, 2,
//		3, 4, 5
//	};
//
//	Mango::Model quad_model(GL_TRIANGLES, 6, GL_UNSIGNED_INT, quad_indices);
//	{
//		quad_model.GetVAO().Bind();
//		auto* vbo = &quad_model.AddVBO();
//		vbo->Setup(sizeof(float) * 4 * 6, quad_vertices);
//		vbo->Bind();
//		Mango::VertexArray::EnableAttribute(0, 2, GL_FLOAT, false, sizeof(float) * 4, 0);
//		Mango::VertexArray::EnableAttribute(1, 2, GL_FLOAT, false, sizeof(float) * 4, sizeof(float) * 2);
//	}
//
//	Mango::Model cube_model;
//	if (!LoadModel(cube_model, "res/models/cube.obj"))
//	{
//		DBG_ERROR("Failed to load cube model");
//		mango.Release();
//		system("pause");
//		return EXIT_FAILURE;
//	}
//
//	Mango::Model model;
//	if (!LoadModel(model, "res/models/cube.obj"))
//	{
//		DBG_ERROR("Failed to load model");
//		mango.Release();
//		system("pause");
//		return EXIT_FAILURE;
//	}
//
//	Mango::Model skybox_model;
//	Mango::CubeTexture skybox_texture;
//	LoadCubeMap(skybox_model, skybox_texture, { "res/textures/skybox_2/right.jpg", 
//		"res/textures/skybox_2/left.jpg", 
//		"res/textures/skybox_2/top.jpg", 
//		"res/textures/skybox_2/bottom.jpg", 
//		"res/textures/skybox_2/back.jpg",
//		"res/textures/skybox_2/front.jpg" 
//	});
//
//	Mango::Entity3D object({ 0.f, 0.f, 0.f });
//	Mango::Camera3D camera({ 0.f, 0.f, 1.f }, { 0.f, 0.f, 0.f });
//	Mango::Light3D light({ -5.f, 5.f, 5.f });
//	light.SetScale(0.2f);
//	light.SetAmbientColor({ 0.1f, 0.1f, 0.1f });
//	light.SetDiffuseColor({ 1.f, 1.f, 1.f });
//	light.SetSpecularColor({ 1.f, 1.f, 1.f });
//
//	Mango::Material3D cube_material;
//	cube_material.shine_damper = 32.f;
//
//	Mango::Shader post_process_shader(Mango::Shader::ReadFile("res/shaders/post_process_vs.glsl"),
//		Mango::Shader::ReadFile("res/shaders/post_process_fs.glsl"));
//	Mango::Shader flat_shader(Mango::Shader::ReadFile("res/shaders/flat_vs.glsl"),
//		Mango::Shader::ReadFile("res/shaders/flat_fs.glsl"));
//	Mango::Shader phong_shader(Mango::Shader::ReadFile("res/shaders/phong_vs.glsl"),
//		Mango::Shader::ReadFile("res/shaders/phong_fs.glsl"));
//	Mango::Shader blinn_phong_shader(Mango::Shader::ReadFile("res/shaders/blinn_phong_vs.glsl"),
//		Mango::Shader::ReadFile("res/shaders/blinn_phong_fs.glsl"));
//	Mango::Shader skybox_shader(Mango::Shader::ReadFile("res/shaders/skybox_vs.glsl"),
//		Mango::Shader::ReadFile("res/shaders/skybox_fs.glsl"));
//
//	Mango::Texture diffuse_map("res/textures/diffuse_map.png", true);
//	Mango::Texture specular_map("res/textures/specular_map.png");
//
//	framebuffer.Setup({ 800, 600 });
//
//	Mango::ECS::ECSCore ecs_core;
//
//	const auto entity = ecs_core.CreateEntity();
//	const auto entity2 = ecs_core.CreateEntity();
//
//	ecs_core.AttachComponent<PositionComponent>(entity, 1.f, 2.f, 3.f);
//	ecs_core.AttachComponent<VelocityComponent>(entity, 1.f, 1.f, 1.f);
//
//	ecs_core.AttachSystem(&movement_system);
//	ecs_core.AttachSystem(&render_system);
//	ecs_core.AttachSystem(&debug_system);
//
//	glm::vec3 border_color = { 1.f, 1.f, 1.f };
//	while (mango.NextFrame())
//	{
//		ecs_core.OnTick();
//		ecs_core.OnFrame();
//
//		HandleCamera(mango, camera);
//
//		// ImGui
//		{
//			ImGui::Begin("Mango");
//			ImGui::Text("FPS: %.0f", 1.f / mango.GetFrameTime());
//			if (static bool c = true; ImGui::Checkbox("Vertical Sync", &c))
//				mango.SetVerticalSync(c);
//
//			// object
//			ImGui::Separator();
//			ImGui::TextColored(ImColor(0.f, 1.f, 1.f), "Object");
//			if (glm::vec3 value = object.GetPosition(); ImGui::InputFloat3("Position##object", &value[0]))
//				object.SetPosition(value);
//			if (glm::vec3 value = object.GetRotation(); ImGui::InputFloat3("Rotation##object", &value[0]))
//				object.SetRotation(value);
//			if (float value = object.GetScale(); ImGui::InputFloat("Scale##object", &value))
//				object.SetScale(value);
//
//			static char model_string[256] = { "res/models/cube.obj" };
//			ImGui::InputText("Model##object model", model_string, 255);
//			ImGui::SameLine();
//			if (ImGui::Button("Load##object model"))
//			{
//				model.Release();
//				if (!LoadModel(model, model_string))
//				{
//					DBG_ERROR("Failed to load model");
//					mango.Release();
//					system("pause");
//					return EXIT_FAILURE;
//				}
//			}
//
//			ImGui::ColorEdit3("Border color##object border color", &border_color[0]);
//
//			if (static bool value = false; ImGui::Checkbox("Rotate##object", &value) || value)
//				object.SetRotation({ sin(glfwGetTime()) * glm::pi<float>(), sin(glfwGetTime()) * glm::pi<float>(), sin(glfwGetTime()) * glm::pi<float>() });
//
//			// material
//			ImGui::Separator();
//			ImGui::TextColored(ImColor(0.f, 1.f, 1.f), "Material");
//			if (float value = cube_material.shine_damper; ImGui::InputFloat("Shine damper##material", &value))
//				cube_material.shine_damper = value;
//
//			static char diffuse_map_string[256] = { "res/textures/diffuse_map.png" };
//			ImGui::InputText("Diffuse map##texture diffuse map", diffuse_map_string, 255);
//			ImGui::SameLine();
//			if (ImGui::Button("Load##texture diffuse map"))
//			{
//				diffuse_map.Release();
//				diffuse_map.Setup(diffuse_map_string);
//			}
//
//			static char specular_map_string[256] = { "res/textures/specular_map.png" };
//			ImGui::InputText("Specular map##texture specular map", specular_map_string, 255);
//			ImGui::SameLine();
//			if (ImGui::Button("Load##texture specular map"))
//			{
//				specular_map.Release();
//				specular_map.Setup(specular_map_string);
//			}
//
//			// light
//			ImGui::Separator();
//			ImGui::TextColored(ImColor(0.f, 1.f, 1.f), "Light");
//			if (ImGui::Button("Set position"))
//				light.SetPosition(camera.GetPosition());
//			if (glm::vec3 value = light.GetPosition(); ImGui::InputFloat3("Position##light", &value[0]))
//				light.SetPosition(value);
//			if (glm::vec3 value = light.GetAmbientColor(); ImGui::ColorEdit3("Ambient color##light", &value[0]))
//				light.SetAmbientColor(value);
//			if (glm::vec3 value = light.GetDiffuseColor(); ImGui::ColorEdit3("Diffuse color##light", &value[0]))
//				light.SetDiffuseColor(value);
//			if (glm::vec3 value = light.GetSpecularColor(); ImGui::ColorEdit3("Specular color##light", &value[0]))
//				light.SetSpecularColor(value);
//
//			ImGui::End();
//		}
//
//		framebuffer.Bind();
//		glClearColor(0.f, 0.f, 0.f, 1.f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//		glDisable(GL_STENCIL_TEST);
//
//		// light cube
//		{
//			cube_model.GetVAO().Bind();
//
//			flat_shader.Bind();
//			flat_shader.SetUniformMat4("u_projection_matrix", Mango::Maths::CreateProjectionMatrix(60.f, mango.GetAspectRatio(), 0.1f, 300.f));
//			flat_shader.SetUniformMat4("u_view_matrix", camera.GetViewMatrix());
//			flat_shader.SetUniformMat4("u_model_matrix", light.GetModelMatrix());
//			flat_shader.SetUniformF3("flat_color", 1.f, 1.f, 1.f);
//			glDrawElements(model.GetMode(), model.GetIBO().GetCount(), model.GetIBO().GetType(), nullptr);
//		}
//
//		// our model
//		{
//			model.GetVAO().Bind();
//
//			glEnable(GL_STENCIL_TEST);
//
//			glStencilFunc(GL_ALWAYS, 1, 0xFF);
//			glStencilMask(0xFF);
//			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//
//			{
//				blinn_phong_shader.Bind();
//
//				// matrices
//				blinn_phong_shader.SetUniformMat4("u_projection_matrix", Mango::Maths::CreateProjectionMatrix(60.f, mango.GetAspectRatio(), 0.1f, 300.f));
//				blinn_phong_shader.SetUniformMat4("u_view_matrix", camera.GetViewMatrix());
//				blinn_phong_shader.SetUniformMat4("u_model_matrix", object.GetModelMatrix());
//				blinn_phong_shader.SetUniformMat3("u_normal_matrix", Mango::Maths::CreateNormalMatrix(object.GetModelMatrix()));
//
//				// material
//				blinn_phong_shader.SetUniformF1("u_material.shine_damper", cube_material.shine_damper);
//				blinn_phong_shader.SetUniformI1("u_material.diffuse_map", 0);
//				blinn_phong_shader.SetUniformI1("u_material.specular_map", 1);
//
//				// light
//				blinn_phong_shader.SetUniformF3("u_light.position", light.GetPosition().x, light.GetPosition().y, light.GetPosition().z);
//				blinn_phong_shader.SetUniformF3("u_light.ambient", light.GetAmbientColor().r, light.GetAmbientColor().g, light.GetAmbientColor().b);
//				blinn_phong_shader.SetUniformF3("u_light.diffuse", light.GetDiffuseColor().r, light.GetDiffuseColor().g, light.GetDiffuseColor().b);
//				blinn_phong_shader.SetUniformF3("u_light.specular", light.GetSpecularColor().r, light.GetSpecularColor().g, light.GetSpecularColor().b);
//
//				// camera
//				blinn_phong_shader.SetUniformF3("u_camera_position", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
//
//				// textures
//				diffuse_map.Bind(0);
//				specular_map.Bind(1);
//
//				glDrawElements(model.GetMode(), model.GetIBO().GetCount(), model.GetIBO().GetType(), nullptr);
//			}
//
//			
//			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//			glStencilMask(0xFF);
//
//			// glow
//			{
//				flat_shader.Bind();
//				flat_shader.SetUniformMat4("u_model_matrix", Mango::Maths::CreateModelMatrix(object.GetPosition(), object.GetRotation(), object.GetScale() + 0.02f));
//				flat_shader.SetUniformF3("flat_color", border_color.r, border_color.g, border_color.b);
//			
//				glDrawElements(model.GetMode(), model.GetIBO().GetCount(), model.GetIBO().GetType(), nullptr);
//			}
//
//			glDisable(GL_STENCIL_TEST);
//		}
//
//		// skybox 
//		{
//			skybox_shader.Bind();
//
//			skybox_shader.SetUniformMat4("u_projection_matrix", Mango::Maths::CreateProjectionMatrix(60.f, mango.GetAspectRatio(), 0.1f, 300.f));
//			skybox_shader.SetUniformMat4("u_view_matrix", glm::mat4(glm::mat3(camera.GetViewMatrix())));
//
//			skybox_texture.Bind();
//			skybox_model.GetVAO().Bind();
//
//			glDrawElements(skybox_model.GetMode(), skybox_model.GetIBO().GetCount(), skybox_model.GetIBO().GetType(), nullptr);
//		}
//
//		// render framebuffer
//		{
//			Mango::Framebuffer::Unbind();
//			
//			framebuffer.GetTexture().Bind();
//
//			post_process_shader.Bind();
//
//			quad_model.GetVAO().Bind();
//			glDrawElements(quad_model.GetMode(), quad_model.GetIBO().GetCount(), quad_model.GetIBO().GetType(), nullptr);
//
//		}
//		mango.EndFrame();
//	}
//
//	ecs_core.Release();
//	mango.Release();
//	system("pause");
//	return EXIT_SUCCESS;
//}