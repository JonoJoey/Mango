#include "player.h"

#include "mango_app.h"

#include "world.h"


void Player::Init()
{
	Mango::LoadCubeModel(m_model);

	m_shader = &*Mango::RescourcePool<Mango::Shader>::Get()->GetOrAddRes("player_shader",
		Mango::Shader::ReadFile("res/shaders/player_vs.glsl"),
		Mango::Shader::ReadFile("res/shaders/player_fs.glsl"));
}
void Player::Release()
{
	m_model.Release();
}
void Player::Render(Mango::MangoCore* mango_core, float lerp)
{
	const float tick_interval = GetMangoApp()->GetTickInterval();
	auto& camera = mango_core->GetRenderer3D().GetCamera();
	auto position = GetPosition();
	auto velocity = GetVelocity();
	bool on_ground = IsOnGround();
	SimulateMovement(position, velocity, GetAcceleration(), on_ground);


	m_shader->Bind();
	m_shader->SetUniformMat4("u_projection_matrix", mango_core->GetRenderer3D().GetProjMatrix());
	m_shader->SetUniformMat4("u_view_matrix", camera.GetViewMatrix());
	m_shader->SetUniformMat4("u_model_matrix", Mango::Maths::CreateModelMatrix(GetPosition() + ((position - GetPosition()) * lerp) + glm::vec3(0.5f)));

	m_model.GetVAO().Bind();

	glDrawElements(m_model.GetMode(), m_model.GetIBO().GetCount(), m_model.GetIBO().GetType(), nullptr);

	Mango::Shader::Unbind();
	Mango::CubeTexture::Unbind();
	Mango::VertexArray::Unbind();
}
void Player::FrameUpdate(Mango::MangoCore* mango_core, float lerp)
{

}
void Player::Update()
{
	m_prev_position = m_position;
	m_prev_velocity = m_velocity;
	m_prev_acceleration = m_acceleration;
	m_prev_viewangle = m_viewangle;

	SimulateMovement(m_position, m_velocity, m_acceleration, m_on_ground);
}

void Player::SimulateMovement(glm::vec3& position, glm::vec3& velocity, glm::vec3 acceleration, bool& on_ground)
{
	const float tick_interval = GetMangoApp()->GetTickInterval();

	// gravity
	if (on_ground)
	{
		acceleration.y = 0.f;
		velocity.y = glm::max<float>(velocity.y, 0.f);
	}
	else
		acceleration.y = -m_gravity;

	velocity += acceleration * tick_interval;

	// friction
	if (const float speed = glm::length(velocity); speed > 0.f)
	{
		if (acceleration.x == 0.f)
			velocity.x = (velocity.x / speed) * glm::max<float>(0.f, speed - (m_friction * tick_interval));

		if (acceleration.z == 0.f)
			velocity.z = (velocity.z / speed) * glm::max<float>(0.f, speed - (m_friction * tick_interval));
	}

	// clamp speed
	if (m_speed > m_max_speed)
		m_speed = m_max_speed;

	// clamp velocity
	if (const float vel_length = sqrtf((m_velocity.x * m_velocity.x) + (m_velocity.z * m_velocity.z)); vel_length > m_speed)
	{
		velocity.x = (velocity.x / vel_length) * m_speed;
		velocity.z = (velocity.z / vel_length) * m_speed;
	}

	auto new_position = position + (velocity * tick_interval);
	auto ray_tracer = GetMangoApp()->GetWorld()->GetRayTracer();

	static const glm::vec3 vertex_offsets[8] =
	{
		{ 0.f, 0.f, 0.f },
		{ 1.f, 0.f, 0.f },
		{ 0.f, 1.f, 0.f },
		{ 0.f, 0.f, 1.f },
		{ 1.f, 0.f, 1.f },
		{ 1.f, 1.f, 0.f },
		{ 0.f, 1.f, 1.f },
		{ 1.f, 1.f, 1.f }
	};

	// check if on ground
	if (on_ground = false; velocity.y < 0.f)
	{
		float frac = 1.f;
		for (int i = 0; i < 8; i++)
		{
			TraceInfo trace_info;
			Ray ray(position + vertex_offsets[i], { 0.f, -1.f, 0.f }, -velocity.y * tick_interval, 0.01f);
			if (!ray_tracer->Trace(ray, trace_info))
				continue;

			if (trace_info.fraction >= frac)
				continue;

			frac = trace_info.fraction;
			on_ground = true;
		}

		if (on_ground)
			new_position.y = position.y + velocity.y * frac * tick_interval;
	}

	position = new_position;
}


void LocalPlayer::OnFrameUpdate(Mango::MangoCore* mango_core, float lerp)
{
	auto mango_app = GetMangoApp();
	auto& camera = mango_core->GetRenderer3D().GetCamera();
	auto input_handler = mango_app->GetInputHandler();
	const float tick_interval = mango_app->GetTickInterval();

	// mouse input
	{
		const auto screen_center = mango_core->GetWindowSize() / 2;

		static bool toggle = false;
		if (toggle)
		{
			const auto offset = glm::vec2(mango_core->GetMousePosition().x - screen_center.x, mango_core->GetMousePosition().y - screen_center.y);
			const auto new_viewangle = GetViewangle() + glm::vec3(offset[0], -offset[1], 0.f) * 0.5f;
			SetViewangle(new_viewangle);
			mango_core->SetMousePosition(screen_center);
		}

		// toggle
		if (input_handler->GetButtonState(2) == Mango::INPUT_STATE::INPUT_STATE_PRESS)
		{
			toggle = !toggle;
			if (toggle)
				mango_core->SetMousePosition(screen_center);
		}
	}

	// camera position is extrapolated for that smoooooooooooothness
	{
		auto position = GetPosition();
		auto velocity = GetVelocity();
		bool on_ground = IsOnGround();
		SimulateMovement(position, velocity, GetAcceleration(), on_ground);

		position = GetPosition() + ((position - GetPosition()) * lerp);
		position += glm::vec3(0.5f, m_view_offset, 0.5f);

		if (m_third_person)
		{
			const auto direction = Mango::Maths::AngleVector(GetViewangle());
			camera.SetPosition(position - (direction * m_third_person_distance));
		}
		else
			camera.SetPosition(position);
	}

	camera.SetViewangle(GetViewangle());
	FrameUpdate(mango_core, lerp);
}
void LocalPlayer::OnRender(Mango::MangoCore* mango_core, float lerp)
{
	if (m_third_person)
		Render(mango_core, lerp);
}
void LocalPlayer::OnUpdate()
{
	auto mango_app = GetMangoApp();
	auto world = mango_app->GetWorld();
	auto input_handler = mango_app->GetInputHandler();
	const float tick_interval = GetMangoApp()->GetTickInterval();
	const auto position = GetPosition() + glm::vec3(0.5f, m_view_offset, 0.5f);

	// handle movement
	{
		static const glm::vec3 up_dir = { 0.f, 1.f, 0.f };
		const auto forward_dir = Mango::Maths::AngleVector({ GetViewangle().x, 0.f, 0.f });
		const auto right_dir = glm::normalize(glm::cross(forward_dir, up_dir));

		auto acceleration = glm::vec3(0.f, 0.f, 0.f);
		if (input_handler->GetKeyState('W'))
			acceleration += forward_dir;
		if (input_handler->GetKeyState('S'))
			acceleration += -forward_dir;
		if (input_handler->GetKeyState('D'))
			acceleration += right_dir;
		if (input_handler->GetKeyState('A'))
			acceleration += -right_dir;
		if (input_handler->GetKeyState(GLFW_KEY_SPACE) && IsOnGround())
			SetVelocity(glm::vec3(GetVelocity().x, 6.f, GetVelocity().z));
		if (input_handler->GetKeyState(GLFW_KEY_LEFT_SHIFT))
			acceleration += -up_dir;
		if (input_handler->GetKeyState(GLFW_KEY_F5) == Mango::INPUT_STATE::INPUT_STATE_PRESS)
			SetThirdPerson(!IsThirdPerson());
		if (input_handler->GetKeyState(GLFW_KEY_LEFT_CONTROL))
			SetSpeed(6.5f);
		else
			SetSpeed(4.3f);

		if (const float length = glm::length(acceleration); length > 0.f)
			acceleration = glm::normalize(acceleration) * GetAccelSpeed();

		SetAcceleration(acceleration);
	}

	// placing/breaking blocks
	{
		const auto direction = Mango::Maths::AngleVector(GetViewangle());

		Ray ray(position, direction, 10.f, 0.01f);
		if (TraceInfo trace_info; world->GetRayTracer()->Trace(ray, trace_info) && trace_info.block_face != NUM_BLOCK_FACES)
		{
			int block_x = int(floorf(trace_info.trace_end.x + direction.x * 0.01f)),
				block_y = int(floorf(trace_info.trace_end.y + direction.y * 0.01f)),
				block_z = int(floorf(trace_info.trace_end.z + direction.z * 0.01f));

			// place block
			if (input_handler->GetButtonState(1) == Mango::INPUT_STATE::INPUT_STATE_PRESS)
			{
				switch (trace_info.block_face)
				{
				case BLOCK_FACE_FRONT:
					block_z += 1;
					break;
				case BLOCK_FACE_BACK:
					block_z -= 1;
					break;
				case BLOCK_FACE_RIGHT:
					block_x += 1;
					break;
				case BLOCK_FACE_LEFT:
					block_x -= 1;
					break;
				case BLOCK_FACE_TOP:
					block_y += 1;
					break;
				case BLOCK_FACE_BOTTOM:
					block_y -= 1;
					break;
				}

				world->EditBlock(block_x, block_y, block_z, Block::Create(mango_app->GetBlockMap().at(m_selected_block)));
			}

			// break block
			if (input_handler->GetButtonState(0) == Mango::INPUT_STATE::INPUT_STATE_PRESS)
			{
				DBG_LOG("%i %i %i", block_x, block_y, block_z);
				world->EditBlock(block_x, block_y, block_z, Block::Inactive());
			}
		}
	}

	Update();

	//DBG_LOG("speed:%f accel:%f", glm::length(GetVelocity()), glm::length(GetAcceleration()));
}