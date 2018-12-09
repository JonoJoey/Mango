#include "player.h"

#include "mango_app.h"

#include "world.h"


void Player::Init()
{
	Mango::LoadCubeModel(m_model);

	const auto app_data = Mango::GetAppDataPath();

	m_shader = &*Mango::RescourcePool<Mango::Shader>::Get()->GetOrAddRes("player_shader",
		Mango::Shader::ReadFile(app_data + "/.mango/resource_packs/default/shaders/player_vs.glsl"),
		Mango::Shader::ReadFile(app_data + "/.mango/resource_packs/default/shaders/player_fs.glsl"));
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
	m_shader->SetUniformMat4("u_view_matrix", Mango::Maths::CreateViewMatrix({ 0.f, 0.f, 0.f }, camera.GetViewangle()));
	m_shader->SetUniformMat4("u_model_matrix", Mango::Maths::CreateModelMatrix(GetPosition() + 
		((position - GetPosition()) * double(lerp)) + glm::dvec3(0.5) - glm::dvec3(camera.GetPosition())));

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

	auto ray_tracer = GetMangoApp()->GetWorld()->GetRayTracer();
	Ray ray(GetPosition() + 0.5, { 0.0, -1.0, 0.0 }, glm::dvec3(0.5), 0.1);
	if (TraceInfo trace_info; ray_tracer->Trace(ray, trace_info) && trace_info.m_fraction <= 0.0)
		m_on_ground = true;
	else
		m_on_ground = false;
}

void Player::SimulateMovement(glm::dvec3& position, glm::dvec3& velocity, glm::dvec3 acceleration, bool& on_ground)
{
	const double tick_interval = double(GetMangoApp()->GetTickInterval());
	const auto ray_tracer = GetMangoApp()->GetWorld()->GetRayTracer();

	// gravity
	acceleration.y = -m_gravity;

	velocity += acceleration * tick_interval;

	// friction
	if (const auto speed = std::sqrt((m_velocity.x * m_velocity.x) + (m_velocity.z * m_velocity.z)); speed > 0.0)
	{
		if (acceleration.x == 0.0)
			velocity.x = (velocity.x / speed) * std::max(0.0, speed - (m_friction * tick_interval));

		if (acceleration.z == 0.0)
			velocity.z = (velocity.z / speed) * std::max(0.0, speed - (m_friction * tick_interval));
	}

	// clamp velocity
	if (const auto vel_length = std::sqrt((m_velocity.x * m_velocity.x) + (m_velocity.z * m_velocity.z)); vel_length > m_max_speed)
	{
		velocity.x = (velocity.x / vel_length) * m_max_speed;
		velocity.z = (velocity.z / vel_length) * m_max_speed;
	}

	// collision stuffs
	{
		auto velocity_copy = velocity;
		auto new_position = position;

		for (int i = 0; i < 9; i++)
		{
			if (glm::length(velocity_copy) <= 0.0)
				break;
		
			Ray ray = Ray(new_position + glm::dvec3(0.5, 0.5, 0.5), velocity_copy, glm::dvec3(0.5, 0.5, 0.5), glm::length(velocity_copy) * tick_interval);
			if (TraceInfo trace_info; ray_tracer->Trace(ray, trace_info))
			{
				new_position += velocity_copy * trace_info.m_fraction * tick_interval;

				velocity_copy -= velocity_copy * trace_info.m_fraction;
				velocity_copy -= velocity_copy * trace_info.m_normal * trace_info.m_normal;
			}
			else
			{
				new_position += velocity_copy * tick_interval;
				break;
			}
		}
		
		velocity = (new_position - position) / tick_interval;
		position = position + velocity * tick_interval;
	}
}


void LocalPlayer::OnInit()
{
	Init();

	ASSERT(m_inventory.Setup(GetMangoApp()->GetMangoCore()));
}
void LocalPlayer::OnRelease()
{
	Release();

	m_inventory.Release();
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
			const auto new_viewangle = Mango::Maths::NormalizeAngle(GetViewangle() + glm::dvec3(double(offset[0]), double(-offset[1]), 0.0) * 0.5);
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

		position = GetPosition() + ((position - GetPosition()) * double(lerp));
		position += glm::dvec3(0.5, m_view_offset, 0.5);

		if (m_third_person)
		{
			const auto direction = Mango::Maths::AngleVector(GetViewangle());
			camera.SetPosition(position - glm::dvec3(direction * m_third_person_distance));
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
	auto ray_tracer = mango_app->GetWorld()->GetRayTracer();
	const float tick_interval = GetMangoApp()->GetTickInterval();
	const auto position = GetPosition() + glm::dvec3(0.5, double(m_view_offset), 0.5);

	// handle movement
	{
		static const glm::dvec3 up_dir = { 0.0, 1.0, 0.0 };
		const auto forward_dir = glm::dvec3(Mango::Maths::AngleVector({ float(GetViewangle().x), 0.f, 0.f }));
		const auto right_dir = glm::normalize(glm::cross(forward_dir, up_dir));

		auto acceleration = glm::dvec3(0.0, 0.0, 0.0);
		if (input_handler->GetKeyState('W'))
			acceleration += forward_dir;
		if (input_handler->GetKeyState('S'))
			acceleration += -forward_dir;
		if (input_handler->GetKeyState('D'))
			acceleration += right_dir;
		if (input_handler->GetKeyState('A'))
			acceleration += -right_dir;
		if (input_handler->GetKeyState(GLFW_KEY_SPACE) && IsOnGround())
			SetVelocity(glm::dvec3(GetVelocity().x, 8.0, GetVelocity().z));
		if (input_handler->GetKeyState(GLFW_KEY_F5) == Mango::INPUT_STATE::INPUT_STATE_PRESS)
			SetThirdPerson(!IsThirdPerson());
		if (input_handler->GetKeyState(GLFW_KEY_LEFT_CONTROL))
			SetMaxSpeed(6.5);
		else
			SetMaxSpeed(4.3);

		for (size_t i = 0; i < 9; i++)
		{
			if (input_handler->GetKeyState(GLFW_KEY_1 + i) == Mango::INPUT_STATE::INPUT_STATE_PRESS)
				m_inventory.SetSelectedSlot(i);
		}

		if (const auto length = glm::length(acceleration); length > 0.0)
			acceleration = (acceleration / length) * GetAccelSpeed();

		SetAcceleration(acceleration);
	}

	// placing/breaking blocks
	{
		const auto direction = glm::dvec3(Mango::Maths::AngleVector(GetViewangle()));
		
		Ray ray(position, direction, 10.0);
		if (TraceInfo trace_info; world->GetRayTracer()->Trace(ray, trace_info))
		{
			const auto trace_end = ray.m_start + ray.m_direction * (ray.m_length * trace_info.m_fraction);
			int block_x = int(std::floor(trace_end.x + direction.x * 0.001)),
				block_y = int(std::floor(trace_end.y + direction.y * 0.001)),
				block_z = int(std::floor(trace_end.z + direction.z * 0.001));
		
			// place block
			if (input_handler->GetButtonState(1) == Mango::INPUT_STATE::INPUT_STATE_PRESS)
			{
				block_x += int(trace_info.m_normal.x);
				block_y += int(trace_info.m_normal.y);
				block_z += int(trace_info.m_normal.z);
		
				const auto item = m_inventory.GetSelectedItem();
				if (m_inventory.RemoveItems(m_inventory.GetSelectedSlot(), 1) > 0)
					world->EditBlock(block_x, block_y, block_z, Block::Create(item.m_item_type));
			}
		
			// break block
			if (input_handler->GetButtonState(0) == Mango::INPUT_STATE::INPUT_STATE_PRESS)
			{
				if (Block block; world->GetBlock(block_x, block_y, block_z, block))
					m_inventory.AddItems(block.m_block_id, 1);

				world->EditBlock(block_x, block_y, block_z, Block::Inactive());
			}
		}
	}

	Update();

	//DBG_LOG("speed:%f accel:%f", glm::length(GetVelocity()), glm::length(GetAcceleration()));
}