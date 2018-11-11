#include <Windows.h>

#include "mango/mango.h"


class MangoInput : public Mango::IInputHandler
{
public:
	MangoInput() { for (int i = 0; i < 8; i++) m_button_states[i] = false; }

	void OnKeyPress(int key, std::string key_name, bool repeat)
	{
		m_key_states[key] = true;
	}
	void OnKeyRelease(int key, std::string key_name)
	{
		m_key_states[key] = false;
	}
	void OnMouseMove(float xpos, float ypos)
	{

	}
	void OnMouseButtonPress(int button, bool repeat)
	{
		m_button_states[button] = true;
	}
	void OnMouseButtonRelease(int button)
	{
		m_button_states[button] = false;
	}


	bool GetKeyState(int key)
	{
		auto res = m_key_states.find(key);
		if (res == m_key_states.end())
			return m_key_states[key] = false;
		return res->second;
	}
	bool GetMouseButtonState(int button)
	{
		ASSERT(button >= 0 && button < 8);
		return m_button_states[button];
	}

private:
	std::unordered_map<int, bool> m_key_states;
	bool m_button_states[8];
} g_mango_input;


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

void HandleCamera(Mango::MangoCore& mango, Mango::Camera3D& camera)
{
	static constexpr float move_speed = 2.f; // units per second
	static constexpr float camera_speed = 0.01f;

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
	if (g_mango_input.GetKeyState(GLFW_KEY_ENTER))
	{
		mouse_toggle = true;
		mango.SetMousePosition(mango.GetWindowSize() / 2);
	}
	if (g_mango_input.GetKeyState(GLFW_KEY_ESCAPE))
		mouse_toggle = false;
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

	Mango::Model cube_model;
	if (!LoadModel(cube_model, "res/models/dragon.obj"))
	{
		DBG_ERROR("Failed to load model");
		mango.Release();
		system("pause");
		return EXIT_FAILURE;
	}

	Mango::Camera3D camera({ 0.f, 0.f, 1.f }, { 0.f, 0.f, 0.f });

	Mango::Shader shader(Mango::Shader::ReadFile("res/shaders/basic_vs.glsl"),
		Mango::Shader::ReadFile("res/shaders/basic_fs.glsl"));

	Mango::Texture texture("res/textures/mango.jpg");
	
	while (mango.NextFrame({ 0.f, 1.f, 1.f }))
	{
		ImGui::Begin("Mango");
		ImGui::Text("FPS: %.0f", 1.f / mango.GetFrameTime());
		if (static bool c = true; ImGui::Checkbox("Vertical Sync", &c))
			mango.SetVerticalSync(c);
		ImGui::End();

		HandleCamera(mango, camera);

		cube_model.GetVAO().Bind();
		texture.Bind(0);

		shader.Bind();
		shader.SetUniformMat4("u_projection_matrix", Mango::Maths::CreateProjectionMatrix(60.f, mango.GetAspectRatio(), 0.1f, 300.f));
		shader.SetUniformMat4("u_view_matrix", camera.GetViewMatrix());
		shader.SetUniformMat4("u_model_matrix", Mango::Maths::CreateModelMatrix({ 0.f, 0.f, -1.f }, { 0.f, 0.f, 0.f }));

		glDrawElements(cube_model.GetMode(), cube_model.GetIBO().GetCount(), cube_model.GetIBO().GetType(), nullptr);

		Mango::Shader::Unbind();
		Mango::VertexArray::Unbind();

		mango.EndFrame();
	}

	mango.Release();
	return EXIT_SUCCESS;
}