#include <Windows.h>

#include "mango/mango.h"


class MangoInput : public Mango::IInputHandler
{
public:
	MangoInput() { for (int i = 0; i < 8; i++) m_button_states[i] = Mango::INPUT_STATE::INPUT_STATE_RELEASE; }

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
} g_mango_input;


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

	if (g_mango_input.GetKeyState(' '))
		camera.Move(up_dir * frame_time * move_speed);
	if (g_mango_input.GetKeyState(GLFW_KEY_LEFT_SHIFT))
		camera.Move(-up_dir * frame_time * move_speed);
	if (g_mango_input.GetKeyState('W'))
		camera.Move(forward_dir * frame_time * move_speed);
	if (g_mango_input.GetKeyState('A'))
		camera.Move(-right_dir * frame_time * move_speed);
	if (g_mango_input.GetKeyState('S'))
		camera.Move(-forward_dir * frame_time * move_speed);
	if (g_mango_input.GetKeyState('D'))
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
	if (g_mango_input.GetKeyState(GLFW_KEY_ESCAPE) == Mango::INPUT_STATE::INPUT_STATE_PRESS)
	{
		mouse_toggle = !mouse_toggle;
		mango.SetMousePosition(mango.GetWindowSize() / 2);
	}
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

	if (!mango.RegisterInputHandler(&g_mango_input))
	{
		DBG_ERROR("Failed to setup input handler");
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

	Mango::Entity3D cube({ 0.f, 0.f, 0.f });
	Mango::Camera3D camera({ 0.f, 0.f, 1.f }, { 0.f, 0.f, 0.f });
	Mango::Light3D light({ -5.f, 5.f, 5.f });
	light.SetScale(0.2f);
	light.SetAmbientColor({ 0.1f, 0.1f, 0.1f });
	light.SetDiffuseColor({ 1.f, 1.f, 1.f });
	light.SetSpecularColor({ 1.f, 1.f, 1.f });

	Mango::Material3D cube_material;
	cube_material.specular_strength = 1.f;
	cube_material.specular_shininess = 32.f;

	Mango::Shader flat_shader(Mango::Shader::ReadFile("res/shaders/flat_vs.glsl"),
		Mango::Shader::ReadFile("res/shaders/flat_fs.glsl"));
	Mango::Shader phong_shader(Mango::Shader::ReadFile("res/shaders/phong_vs.glsl"),
		Mango::Shader::ReadFile("res/shaders/phong_fs.glsl"));

	Mango::Texture diffuse_map("res/textures/mango.jpg");
	
	while (mango.NextFrame({ 0.f, 1.f, 1.f }))
	{
		HandleCamera(mango, camera);

		ImGui::Begin("Mango");
		ImGui::Text("FPS: %.0f", 1.f / mango.GetFrameTime());
		if (static bool c = true; ImGui::Checkbox("Vertical Sync", &c))
			mango.SetVerticalSync(c);

		static char texture_string[256] = { "res/textures/mango.jpg" };
		ImGui::InputText("Texture##texture diffuse map", texture_string, 255);
		ImGui::SameLine();
		if (ImGui::Button("Load texture##texture diffuse map"))
		{
			diffuse_map.Release();
			diffuse_map.Setup(texture_string);
		}

		static char model_string[256] = { "res/models/cube.obj" };
		ImGui::InputText("Model##model", model_string, 255);
		ImGui::SameLine();
		if (ImGui::Button("Load model##model"))
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

		ImGui::Separator();
		ImGui::TextColored(ImColor(0.f, 1.f, 1.f), "Material");
		if (float value = cube_material.specular_strength; ImGui::InputFloat("Specular strength##material", &value))
			cube_material.specular_strength = value;
		if (float value = cube_material.specular_shininess; ImGui::InputFloat("Specular shininess##material", &value))
			cube_material.specular_shininess = value;

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

		model.GetVAO().Bind();

		flat_shader.Bind();
		flat_shader.SetUniformMat4("u_projection_matrix", Mango::Maths::CreateProjectionMatrix(60.f, mango.GetAspectRatio(), 0.1f, 300.f));
		flat_shader.SetUniformMat4("u_view_matrix", camera.GetViewMatrix());
		flat_shader.SetUniformMat4("u_model_matrix", light.GetModelMatrix());
		flat_shader.SetUniformF3("flat_color", light.GetDiffuseColor().r, light.GetDiffuseColor().g, light.GetDiffuseColor().b);
		glDrawElements(model.GetMode(), model.GetIBO().GetCount(), model.GetIBO().GetType(), nullptr);

		phong_shader.Bind();

		cube.SetRotation({ sin(glfwGetTime()) * glm::pi<float>(), sin(glfwGetTime()) * glm::pi<float>(), sin(glfwGetTime()) * glm::pi<float>() });

		// matrices
		phong_shader.SetUniformMat4("u_projection_matrix", Mango::Maths::CreateProjectionMatrix(60.f, mango.GetAspectRatio(), 0.1f, 300.f));
		phong_shader.SetUniformMat4("u_view_matrix", camera.GetViewMatrix());
		phong_shader.SetUniformMat4("u_model_matrix", cube.GetModelMatrix());
		phong_shader.SetUniformMat3("u_normal_matrix", Mango::Maths::CreateNormalMatrix(cube.GetModelMatrix()));

		// material
		phong_shader.SetUniformF1("u_material.specular_strength", cube_material.specular_strength);
		phong_shader.SetUniformF1("u_material.specular_shininess", cube_material.specular_shininess);
		phong_shader.SetUniformI1("u_material.diffuse_map", 0);

		// light
		phong_shader.SetUniformF3("u_light.position", light.GetPosition().x, light.GetPosition().y, light.GetPosition().z);
		phong_shader.SetUniformF3("u_light.ambient", light.GetAmbientColor().r, light.GetAmbientColor().g, light.GetAmbientColor().b);
		phong_shader.SetUniformF3("u_light.diffuse", light.GetDiffuseColor().r, light.GetDiffuseColor().g, light.GetDiffuseColor().b);
		phong_shader.SetUniformF3("u_light.specular", light.GetSpecularColor().r, light.GetSpecularColor().g, light.GetSpecularColor().b);

		// camera
		phong_shader.SetUniformF3("u_camera_position", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// textures
		diffuse_map.Bind(0);

		glDrawElements(model.GetMode(), model.GetIBO().GetCount(), model.GetIBO().GetType(), nullptr);

		Mango::Texture::Unbind();
		Mango::Shader::Unbind();
		Mango::VertexArray::Unbind();

		mango.EndFrame();
	}

	mango.Release();
	return EXIT_SUCCESS;
}