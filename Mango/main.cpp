#include <Windows.h>

#include "mango/mango.h"


Mango::Framebuffer framebuffer;

class MangoEventHandler : public Mango::IEventHandler
{
public:
	MangoEventHandler() { for (int i = 0; i < 8; i++) m_button_states[i] = Mango::INPUT_STATE::INPUT_STATE_RELEASE; }

	void OnKeyPress(int key, std::string key_name, bool repeat)
	{
		m_key_states[key] = repeat ? Mango::INPUT_STATE::INPUT_STATE_REPEAT : Mango::INPUT_STATE::INPUT_STATE_PRESS;
	}
	void OnKeyRelease(int key, std::string key_name)
	{
		m_key_states[key] = Mango::INPUT_STATE::INPUT_STATE_RELEASE;
	}
	void OnMouseMove(float xpos, float ypos)
	{

	}
	void OnMouseButtonPress(int button, bool repeat)
	{
		m_button_states[button] = repeat ? Mango::INPUT_STATE::INPUT_STATE_REPEAT : Mango::INPUT_STATE::INPUT_STATE_PRESS;
	}
	void OnMouseButtonRelease(int button)
	{
		m_button_states[button] = Mango::INPUT_STATE::INPUT_STATE_REPEAT;
	}
	void OnWindowResize(int width, int height)
	{
		glViewport(0, 0, width, height);

		framebuffer.Release();
		framebuffer.Setup({ width, height });
		DBG_LOG("Resize [%i, %i]", width, height);
	}


	Mango::INPUT_STATE GetKeyState(int key)
	{
		auto res = m_key_states.find(key);
		if (res == m_key_states.end())
			return m_key_states[key] = Mango::INPUT_STATE::INPUT_STATE_RELEASE;

		const auto ret = res->second;
		if (ret == Mango::INPUT_STATE::INPUT_STATE_PRESS)
			res->second = Mango::INPUT_STATE::INPUT_STATE_REPEAT;

		return ret;
	}
	Mango::INPUT_STATE GetMouseButtonState(int button)
	{
		ASSERT(button >= 0 && button < 8);

		const auto ret = m_button_states[button];
		if (m_button_states[button] == Mango::INPUT_STATE::INPUT_STATE_PRESS)
			m_button_states[button] = Mango::INPUT_STATE::INPUT_STATE_RELEASE;

		return ret;
	}

private:
	std::unordered_map<int, Mango::INPUT_STATE> m_key_states;
	Mango::INPUT_STATE m_button_states[8];
} g_mango_event_handler;


bool LoadCubeMap(Mango::Model& model, Mango::CubeTexture& texture, std::array<std::string, 6> file_paths)
{
	static const float positions[108] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	static const unsigned int indices[36] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35
	};

	model.Setup(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
	model.GetVAO().Bind();

	// positions
	auto vbo = &model.AddVBO();
	vbo->Setup(108 * sizeof(float), positions);
	vbo->Bind();
	Mango::VertexArray::EnableAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	Mango::VertexArray::Unbind();

	return texture.Setup(file_paths, true);
}
bool LoadModel(Mango::Model& model, const std::string& file_path)
{
	Mango::WavefrontFormat format;
	if (!Mango::LoadWavefront(file_path, format))
		return false;

	model.Setup(GL_TRIANGLES, format.m_indices.size() * 3, GL_UNSIGNED_INT, format.m_indices.data());
	model.GetVAO().Bind();

	// positions
	auto vbo = &model.AddVBO();
	vbo->Setup(format.m_positions.size() * 3 * sizeof(float), format.m_positions.data());
	vbo->Bind();
	Mango::VertexArray::EnableAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// tex coords
	vbo = &model.AddVBO();
	vbo->Setup(format.m_tex_coords.size() * 2 * sizeof(float), format.m_tex_coords.data());
	vbo->Bind();
	Mango::VertexArray::EnableAttribute(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// normals
	vbo = &model.AddVBO();
	vbo->Setup(format.m_normals.size() * 3 * sizeof(float), format.m_normals.data());
	vbo->Bind();
	Mango::VertexArray::EnableAttribute(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	Mango::VertexArray::Unbind();

	return true;
}
void HandleCamera(Mango::MangoCore& mango, Mango::Camera3D& camera)
{
	static constexpr float move_speed = 2.f; // units per second
	static constexpr float camera_speed = 0.002f;

	const auto frame_time = mango.GetFrameTime();

	static const auto up_dir = glm::normalize(glm::vec3(0.f, 1.f, 0.f));
	const auto forward_dir = Mango::Maths::AngleVector(glm::vec3(camera.GetViewangle().x, 0.f, 0.f));
	const auto right_dir = glm::normalize(glm::cross(forward_dir, up_dir));

	if (g_mango_event_handler.GetKeyState(' '))
		camera.Move(up_dir * frame_time * move_speed);
	if (g_mango_event_handler.GetKeyState(GLFW_KEY_LEFT_SHIFT))
		camera.Move(-up_dir * frame_time * move_speed);
	if (g_mango_event_handler.GetKeyState('W'))
		camera.Move(forward_dir * frame_time * move_speed);
	if (g_mango_event_handler.GetKeyState('A'))
		camera.Move(-right_dir * frame_time * move_speed);
	if (g_mango_event_handler.GetKeyState('S'))
		camera.Move(-forward_dir * frame_time * move_speed);
	if (g_mango_event_handler.GetKeyState('D'))
		camera.Move(right_dir * frame_time * move_speed);

	static bool mouse_toggle = false;
	if (mouse_toggle)
	{
		const auto center = mango.GetWindowSize() / 2;
		const auto position = mango.GetMousePosition();

		const auto offset = glm::vec2(position.x - center.x, position.y - center.y);
		const auto viewangle = camera.GetViewangle();
		camera.SetViewangle(viewangle + glm::vec3(offset.x * camera_speed, -offset.y * camera_speed, 0.f));

		mango.SetMousePosition(center);
	}

	// yes this is supposed to be after the above if statement
	if (g_mango_event_handler.GetKeyState(GLFW_KEY_ESCAPE) == Mango::INPUT_STATE::INPUT_STATE_PRESS)
	{
		mouse_toggle = !mouse_toggle;
		mango.SetMousePosition(mango.GetWindowSize() / 2);
	}
}


NEW_COMPONENT(PositionComponent)
{
	float x, y, z;
}; COMPONENT_INFO(PositionComponent, "position");


int main()
{
	Mango::MangoCore mango;
	if (!mango.Setup("Mango", { 800, 600 }))
	{
		DBG_ERROR("Failed to setup mango");
		system("pause");
		return EXIT_FAILURE;
	}

	if (!mango.RegisterEventHandler(&g_mango_event_handler))
	{
		DBG_ERROR("Failed to setup event handler");
		system("pause");
		return EXIT_FAILURE;
	}

	float quad_vertices[] =
	{
		-1.f,  1.f,  0.f, 1.f,
		-1.f, -1.f,  0.f, 0.f,
		 1.f, -1.f,  1.f, 0.f,

		-1.f,  1.f,  0.f, 1.f,
		 1.f, -1.f,  1.f, 0.f,
		 1.f,  1.f,  1.f, 1.f
	};
	unsigned int quad_indices[] =
	{
		0, 1, 2,
		3, 4, 5
	};

	Mango::Model quad_model(GL_TRIANGLES, 6, GL_UNSIGNED_INT, quad_indices);
	{
		quad_model.GetVAO().Bind();
		auto* vbo = &quad_model.AddVBO();
		vbo->Setup(sizeof(float) * 4 * 6, quad_vertices);
		vbo->Bind();
		Mango::VertexArray::EnableAttribute(0, 2, GL_FLOAT, false, sizeof(float) * 4, 0);
		Mango::VertexArray::EnableAttribute(1, 2, GL_FLOAT, false, sizeof(float) * 4, sizeof(float) * 2);
	}

	Mango::Model cube_model;
	if (!LoadModel(cube_model, "res/models/cube.obj"))
	{
		DBG_ERROR("Failed to load cube model");
		mango.Release();
		system("pause");
		return EXIT_FAILURE;
	}

	Mango::Model model;
	if (!LoadModel(model, "res/models/cube.obj"))
	{
		DBG_ERROR("Failed to load model");
		mango.Release();
		system("pause");
		return EXIT_FAILURE;
	}

	Mango::Model skybox_model;
	Mango::CubeTexture skybox_texture;
	LoadCubeMap(skybox_model, skybox_texture, { "res/textures/skybox_2/right.jpg", 
		"res/textures/skybox_2/left.jpg", 
		"res/textures/skybox_2/top.jpg", 
		"res/textures/skybox_2/bottom.jpg", 
		"res/textures/skybox_2/back.jpg",
		"res/textures/skybox_2/front.jpg" 
	});

	Mango::Entity3D object({ 0.f, 0.f, 0.f });
	Mango::Camera3D camera({ 0.f, 0.f, 1.f }, { 0.f, 0.f, 0.f });
	Mango::Light3D light({ -5.f, 5.f, 5.f });
	light.SetScale(0.2f);
	light.SetAmbientColor({ 0.1f, 0.1f, 0.1f });
	light.SetDiffuseColor({ 1.f, 1.f, 1.f });
	light.SetSpecularColor({ 1.f, 1.f, 1.f });

	Mango::Material3D cube_material;
	cube_material.shine_damper = 32.f;

	Mango::Shader post_process_shader(Mango::Shader::ReadFile("res/shaders/post_process_vs.glsl"),
		Mango::Shader::ReadFile("res/shaders/post_process_fs.glsl"));
	Mango::Shader flat_shader(Mango::Shader::ReadFile("res/shaders/flat_vs.glsl"),
		Mango::Shader::ReadFile("res/shaders/flat_fs.glsl"));
	Mango::Shader phong_shader(Mango::Shader::ReadFile("res/shaders/phong_vs.glsl"),
		Mango::Shader::ReadFile("res/shaders/phong_fs.glsl"));
	Mango::Shader blinn_phong_shader(Mango::Shader::ReadFile("res/shaders/blinn_phong_vs.glsl"),
		Mango::Shader::ReadFile("res/shaders/blinn_phong_fs.glsl"));
	Mango::Shader skybox_shader(Mango::Shader::ReadFile("res/shaders/skybox_vs.glsl"),
		Mango::Shader::ReadFile("res/shaders/skybox_fs.glsl"));

	Mango::Texture diffuse_map("res/textures/diffuse_map.png", true);
	Mango::Texture specular_map("res/textures/specular_map.png");

	framebuffer.Setup({ 800, 600 });



	glm::vec3 border_color = { 1.f, 1.f, 1.f };
	while (mango.NextFrame({ 0.f, 0.f, 0.f }))
	{
		HandleCamera(mango, camera);

		// ImGui
		{
			ImGui::Begin("Mango");
			ImGui::Text("FPS: %.0f", 1.f / mango.GetFrameTime());
			if (static bool c = true; ImGui::Checkbox("Vertical Sync", &c))
				mango.SetVerticalSync(c);

			// object
			ImGui::Separator();
			ImGui::TextColored(ImColor(0.f, 1.f, 1.f), "Object");
			if (glm::vec3 value = object.GetPosition(); ImGui::InputFloat3("Position##object", &value[0]))
				object.SetPosition(value);
			if (glm::vec3 value = object.GetRotation(); ImGui::InputFloat3("Rotation##object", &value[0]))
				object.SetRotation(value);
			if (float value = object.GetScale(); ImGui::InputFloat("Scale##object", &value))
				object.SetScale(value);

			static char model_string[256] = { "res/models/cube.obj" };
			ImGui::InputText("Model##object model", model_string, 255);
			ImGui::SameLine();
			if (ImGui::Button("Load##object model"))
			{
				model.Release();
				if (!LoadModel(model, model_string))
				{
					DBG_ERROR("Failed to load model");
					mango.Release();
					system("pause");
					return EXIT_FAILURE;
				}
			}

			ImGui::ColorEdit3("Border color##object border color", &border_color[0]);

			if (static bool value = false; ImGui::Checkbox("Rotate##object", &value) || value)
				object.SetRotation({ sin(glfwGetTime()) * glm::pi<float>(), sin(glfwGetTime()) * glm::pi<float>(), sin(glfwGetTime()) * glm::pi<float>() });

			// material
			ImGui::Separator();
			ImGui::TextColored(ImColor(0.f, 1.f, 1.f), "Material");
			if (float value = cube_material.shine_damper; ImGui::InputFloat("Shine damper##material", &value))
				cube_material.shine_damper = value;

			static char diffuse_map_string[256] = { "res/textures/diffuse_map.png" };
			ImGui::InputText("Diffuse map##texture diffuse map", diffuse_map_string, 255);
			ImGui::SameLine();
			if (ImGui::Button("Load##texture diffuse map"))
			{
				diffuse_map.Release();
				diffuse_map.Setup(diffuse_map_string);
			}

			static char specular_map_string[256] = { "res/textures/specular_map.png" };
			ImGui::InputText("Specular map##texture specular map", specular_map_string, 255);
			ImGui::SameLine();
			if (ImGui::Button("Load##texture specular map"))
			{
				specular_map.Release();
				specular_map.Setup(specular_map_string);
			}

			// light
			ImGui::Separator();
			ImGui::TextColored(ImColor(0.f, 1.f, 1.f), "Light");
			if (ImGui::Button("Set position"))
				light.SetPosition(camera.GetPosition());
			if (glm::vec3 value = light.GetPosition(); ImGui::InputFloat3("Position##light", &value[0]))
				light.SetPosition(value);
			if (glm::vec3 value = light.GetAmbientColor(); ImGui::ColorEdit3("Ambient color##light", &value[0]))
				light.SetAmbientColor(value);
			if (glm::vec3 value = light.GetDiffuseColor(); ImGui::ColorEdit3("Diffuse color##light", &value[0]))
				light.SetDiffuseColor(value);
			if (glm::vec3 value = light.GetSpecularColor(); ImGui::ColorEdit3("Specular color##light", &value[0]))
				light.SetSpecularColor(value);

			ImGui::End();
		}

		framebuffer.Bind();
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glDisable(GL_STENCIL_TEST);

		// light cube
		{
			cube_model.GetVAO().Bind();

			flat_shader.Bind();
			flat_shader.SetUniformMat4("u_projection_matrix", Mango::Maths::CreateProjectionMatrix(60.f, mango.GetAspectRatio(), 0.1f, 300.f));
			flat_shader.SetUniformMat4("u_view_matrix", camera.GetViewMatrix());
			flat_shader.SetUniformMat4("u_model_matrix", light.GetModelMatrix());
			flat_shader.SetUniformF3("flat_color", 1.f, 1.f, 1.f);
			glDrawElements(model.GetMode(), model.GetIBO().GetCount(), model.GetIBO().GetType(), nullptr);
		}

		// our model
		{
			model.GetVAO().Bind();

			glEnable(GL_STENCIL_TEST);

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			{
				blinn_phong_shader.Bind();

				// matrices
				blinn_phong_shader.SetUniformMat4("u_projection_matrix", Mango::Maths::CreateProjectionMatrix(60.f, mango.GetAspectRatio(), 0.1f, 300.f));
				blinn_phong_shader.SetUniformMat4("u_view_matrix", camera.GetViewMatrix());
				blinn_phong_shader.SetUniformMat4("u_model_matrix", object.GetModelMatrix());
				blinn_phong_shader.SetUniformMat3("u_normal_matrix", Mango::Maths::CreateNormalMatrix(object.GetModelMatrix()));

				// material
				blinn_phong_shader.SetUniformF1("u_material.shine_damper", cube_material.shine_damper);
				blinn_phong_shader.SetUniformI1("u_material.diffuse_map", 0);
				blinn_phong_shader.SetUniformI1("u_material.specular_map", 1);

				// light
				blinn_phong_shader.SetUniformF3("u_light.position", light.GetPosition().x, light.GetPosition().y, light.GetPosition().z);
				blinn_phong_shader.SetUniformF3("u_light.ambient", light.GetAmbientColor().r, light.GetAmbientColor().g, light.GetAmbientColor().b);
				blinn_phong_shader.SetUniformF3("u_light.diffuse", light.GetDiffuseColor().r, light.GetDiffuseColor().g, light.GetDiffuseColor().b);
				blinn_phong_shader.SetUniformF3("u_light.specular", light.GetSpecularColor().r, light.GetSpecularColor().g, light.GetSpecularColor().b);

				// camera
				blinn_phong_shader.SetUniformF3("u_camera_position", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

				// textures
				diffuse_map.Bind(0);
				specular_map.Bind(1);

				glDrawElements(model.GetMode(), model.GetIBO().GetCount(), model.GetIBO().GetType(), nullptr);
			}

			
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0xFF);

			// glow
			{
				flat_shader.Bind();
				flat_shader.SetUniformMat4("u_model_matrix", Mango::Maths::CreateModelMatrix(object.GetPosition(), object.GetRotation(), object.GetScale() + 0.02f));
				flat_shader.SetUniformF3("flat_color", border_color.r, border_color.g, border_color.b);
			
				glDrawElements(model.GetMode(), model.GetIBO().GetCount(), model.GetIBO().GetType(), nullptr);
			}

			glDisable(GL_STENCIL_TEST);
		}

		// skybox 
		{
			skybox_shader.Bind();

			skybox_shader.SetUniformMat4("u_projection_matrix", Mango::Maths::CreateProjectionMatrix(60.f, mango.GetAspectRatio(), 0.1f, 300.f));
			skybox_shader.SetUniformMat4("u_view_matrix", glm::mat4(glm::mat3(camera.GetViewMatrix())));

			skybox_texture.Bind();
			skybox_model.GetVAO().Bind();

			glDrawElements(skybox_model.GetMode(), skybox_model.GetIBO().GetCount(), skybox_model.GetIBO().GetType(), nullptr);
		}

		// render framebuffer
		{
			Mango::Framebuffer::Unbind();
			
			framebuffer.GetTexture().Bind();

			post_process_shader.Bind();

			quad_model.GetVAO().Bind();
			glDrawElements(quad_model.GetMode(), quad_model.GetIBO().GetCount(), quad_model.GetIBO().GetType(), nullptr);

		}
		mango.EndFrame();
	}

	mango.Release();
	system("pause");
	return EXIT_SUCCESS;
}