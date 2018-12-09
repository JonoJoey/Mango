#pragma once

#include "entity.h"
#include "inventory.h"


class Player : public Entity
{
public:
	Player() = default;

	void OnInit(std::string resource_pack) override { Init(resource_pack); }
	void OnRelease() override { Release(); }
	void OnRender(Mango::MangoCore* mango_core, float lerp) override { Render(mango_core, lerp); }
	void OnFrameUpdate(Mango::MangoCore* mango_core, float lerp) override { FrameUpdate(mango_core, lerp); }
	void OnUpdate() override { Update(); }

	// getters/setters
	void SetViewangle(const glm::dvec3& viewangle) { m_viewangle = viewangle; }
	glm::dvec3 GetViewangle() const { return m_viewangle; }
	void SetPosition(const glm::dvec3& position) { m_position = position; }
	glm::dvec3 GetPosition() const { return m_position; }
	void SetVelocity(const glm::dvec3& velocity) { m_velocity = velocity; }
	glm::dvec3 GetVelocity() const { return m_velocity; }
	void SetAcceleration(const glm::dvec3& acceleration) { m_acceleration = acceleration; }
	glm::dvec3 GetAcceleration() const { return m_acceleration; }
	void SetPrevViewangle(const glm::dvec3& viewangle) { m_prev_viewangle = viewangle; }
	glm::dvec3 GetPrevViewangle() const { return m_prev_viewangle; }
	void SetPrevPosition(const glm::dvec3& position) { m_prev_position = position; }
	glm::dvec3 GetPrevPosition() const { return m_prev_position; }
	void SetPrevVelocity(const glm::dvec3& velocity) { m_prev_velocity = velocity; }
	glm::dvec3 GetPrevVelocity() const { return m_prev_velocity; }
	void SetPrevAcceleration(const glm::dvec3& acceleration) { m_prev_acceleration = acceleration; }
	glm::dvec3 GetPrevAcceleration() const { return m_prev_acceleration; }

	double GetMaxSpeed() const { return m_max_speed; }
	void SetMaxSpeed(double speed) { m_max_speed = speed; }
	double GetFriction() const { return m_friction; }
	void SetFriction(double friction) { m_friction = friction; }
	double GetAccelSpeed() const { return m_accel_speed; }
	void SetAccelSpeed(double accel_speed) { m_accel_speed = accel_speed; }

	bool IsOnGround() const { return m_on_ground; }

public:
	void Init(std::string resource_pack);
	void Release();
	void Render(Mango::MangoCore* mango_core, float lerp);
	void FrameUpdate(Mango::MangoCore* mango_core, float lerp);
	void Update();

	void SimulateMovement(glm::dvec3& position, glm::dvec3& velocity, glm::dvec3 acceleration, bool& on_ground);

private:
	Mango::Model m_model;
	Mango::Shader* m_shader;

	glm::dvec3 m_viewangle = { 0.0, 0.0, 0.0 },
		m_prev_viewangle = { 0.0, 0.0, 0.0 };
	glm::dvec3 m_position = { 0.0, 0.0, 0.0 },
		m_prev_position = { 0.0, 0.0, 0.0 };
	glm::dvec3 m_velocity = { 0.0, 0.0, 0.0 },
		m_prev_velocity = { 0.0, 0.0, 0.0 };
	glm::dvec3 m_acceleration = { 0.0, 0.0, 0.0 },
		m_prev_acceleration = { 0.0, 0.0, 0.0 };

	// units per second
	double m_max_speed = 4.3;
	double m_friction = 24.0;
	double m_accel_speed = 24.0;
	double m_gravity = 28.0;

	bool m_on_ground = false;
};

// encapsulates instead of inherits from Player
class LocalPlayer : public Player
{
public:
	void OnInit(std::string resource_pack) override;
	void OnRelease() override;
	void OnFrameUpdate(Mango::MangoCore* mango_core, float lerp) override;
	void OnRender(Mango::MangoCore* mango_core, float lerp) override;
	void OnUpdate() override;

public:
	void SetThirdPerson(bool value) { m_third_person = value; }
	bool IsThirdPerson() const { return m_third_person; }

	void SetThirdPersonDistance(float value) { m_third_person_distance = value; }
	float GetThirdPersonDistance() const { return m_third_person_distance; }

	void SetMouseSensitivity(float value) { m_mouse_sensitivity = value; }
	float GetMouseSensitivity() const { return m_mouse_sensitivity; }

	Inventory& GetInventory() { return m_inventory; }

private:
	bool m_third_person = false;
	float m_third_person_distance = 10.f;
	float m_mouse_sensitivity = 1.f;
	float m_view_offset = 2.f;
	Inventory m_inventory;
};