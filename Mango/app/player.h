#pragma once

#include "entity.h"


class Player : public Entity
{
public:
	Player() = default;

	void OnInit() override { Init(); }
	void OnRelease() override { Release(); }
	void OnRender(Mango::MangoCore* mango_core, float lerp) override { Render(mango_core, lerp); }
	void OnFrameUpdate(Mango::MangoCore* mango_core, float lerp) override { FrameUpdate(mango_core, lerp); }
	void OnUpdate() override { Update(); }

	// getters/setters
	void SetViewangle(const glm::vec3& viewangle) { m_viewangle = viewangle; }
	glm::vec3 GetViewangle() const { return m_viewangle; }
	void SetPosition(const glm::vec3& position) { m_position = position; }
	glm::vec3 GetPosition() const { return m_position; }
	void SetVelocity(const glm::vec3& velocity) { m_velocity = velocity; }
	glm::vec3 GetVelocity() const { return m_velocity; }
	void SetAcceleration(const glm::vec3& acceleration) { m_acceleration = acceleration; }
	glm::vec3 GetAcceleration() const { return m_acceleration; }
	void SetPrevViewangle(const glm::vec3& viewangle) { m_prev_viewangle = viewangle; }
	glm::vec3 GetPrevViewangle() const { return m_prev_viewangle; }
	void SetPrevPosition(const glm::vec3& position) { m_prev_position = position; }
	glm::vec3 GetPrevPosition() const { return m_prev_position; }
	void SetPrevVelocity(const glm::vec3& velocity) { m_prev_velocity = velocity; }
	glm::vec3 GetPrevVelocity() const { return m_prev_velocity; }
	void SetPrevAcceleration(const glm::vec3& acceleration) { m_prev_acceleration = acceleration; }
	glm::vec3 GetPrevAcceleration() const { return m_prev_acceleration; }

	float GetMaxSpeed() const { return m_max_speed; }
	void SetMaxSpeed(float speed) { m_max_speed = speed; }
	float GetFriction() const { return m_friction; }
	void SetFriction(float friction) { m_friction = friction; }
	float GetAccelSpeed() const { return m_accel_speed; }
	void SetAccelSpeed(float accel_speed) { m_accel_speed = accel_speed; }

	bool IsOnGround() const { return m_on_ground; }

public:
	void Init();
	void Release();
	void Render(Mango::MangoCore* mango_core, float lerp);
	void FrameUpdate(Mango::MangoCore* mango_core, float lerp);
	void Update();

	void SimulateMovement(glm::vec3& position, glm::vec3& velocity, glm::vec3 acceleration, bool& on_ground);

private:
	Mango::Model m_model;
	Mango::Shader* m_shader;

	glm::vec3 m_viewangle = { 0.f, 0.f, 0.f },
		m_prev_viewangle = { 0.f, 0.f, 0.f };
	glm::vec3 m_position = { 0.f, 0.f, 0.f },
		m_prev_position = { 0.f, 0.f, 0.f };
	glm::vec3 m_velocity = { 0.f, 0.f, 0.f },
		m_prev_velocity = { 0.f, 0.f, 0.f };
	glm::vec3 m_acceleration = { 0.f, 0.f, 0.f },
		m_prev_acceleration = { 0.f, 0.f, 0.f };

	// units per second
	float m_max_speed = 4.3f;
	float m_friction = 16.f;
	float m_accel_speed = 24.f;
	float m_gravity = 16.f;

	bool m_on_ground = false;
};

// encapsulates instead of inherits from Player
class LocalPlayer : public Player
{
public:
	void OnFrameUpdate(Mango::MangoCore* mango_core, float lerp) override;
	void OnRender(Mango::MangoCore* mango_core, float lerp) override;
	void OnUpdate() override;

public:
	void SetThirdPerson(bool value) { m_third_person = value; }
	bool IsThirdPerson() const { return m_third_person; }

	void SetThirdPersonDistance(float value) { m_third_person_distance = value; }
	float GetThirdPersonDistance() const { return m_third_person_distance; }

	void SetSelectedBlock(const std::string& str) { m_selected_block = str; }
	std::string GetSelectedBlock() const { return m_selected_block; }

private:
	std::string m_selected_block = "null";
	bool m_third_person = false;
	float m_third_person_distance = 10.f;
	float m_view_offset = 2.f;
};