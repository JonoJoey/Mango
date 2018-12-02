#pragma once

#include "block.h"



struct Ray
{
	Ray() = default;
	Ray(const glm::vec3& start, const glm::vec3& direction, float length)
		: m_start(start), m_direction(glm::safe_normalize(direction)), m_length(length) {}

	glm::vec3 m_start = { 0.f, 0.f, 0.f },
		m_direction = { 0.f, 0.f, 0.f };
	float m_length = 0.f;
};
struct Plane
{
	Plane() = default;
	Plane(const glm::vec3& point, const glm::vec3& normal)
		: m_point(point), m_normal(glm::safe_normalize(normal)) {}

	glm::vec3 m_point = { 0.f, 0.f, 0.f },
		m_normal = { 0.f, 0.f, 0.f };
};
struct AABB
{
	AABB() = default;
	AABB(const glm::vec3& min, const glm::vec3& max)
		: m_min(min), m_max(max) {}

	glm::vec3 m_min = { 0.f, 0.f, 0.f },
		m_max = { 0.f, 0.f, 0.f };
};

struct TraceInfo
{
	bool m_hit;
	float m_fraction;

	glm::vec3 m_normal,
		m_trace_start,
		m_trace_end;
};

class World;
class RayTracer
{
public:
	bool Trace(const Ray& ray, TraceInfo& trace_info);

	void SetWorld(World* world) { m_world = world; }

private:
	static bool RayPlaneIntersection(const Ray& ray, const Plane& plane, float* intersection_distance = nullptr);
	static bool RayAABBIntersection(const Ray& ray, const AABB& aabb, float* intersection_distance = nullptr, glm::vec3* intersection_normal = nullptr);

private:
	World* m_world = nullptr;
};