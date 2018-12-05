#pragma once

#include "block.h"


struct Ray
{
	Ray() = default;
	Ray(const glm::dvec3& start, const glm::dvec3& direction, double length)
		: m_start(start), m_direction(glm::safe_normalize(direction)), m_length(length), m_extents(0.0), m_is_ray(true) {}
	Ray(const glm::dvec3& start, const glm::dvec3& direction, const glm::dvec3& extents, double length)
		: m_start(start), m_direction(glm::safe_normalize(direction)), m_length(length), m_extents(extents), m_is_ray(false) {}

	glm::dvec3 m_start = { 0.0, 0.0, 0.0 },
		m_direction = { 0.0, 0.0, 0.0 },
		m_extents = { 0.0, 0.0, 0.0 };
	double m_length = 0.0;
	bool m_is_ray = true;
};
struct Plane
{
	Plane() = default;
	Plane(const glm::dvec3& point, const glm::dvec3& normal)
		: m_point(point), m_normal(glm::safe_normalize(normal)) {}

	glm::dvec3 m_point = { 0.0, 0.0, 0.0 },
		m_normal = { 0.0, 0.0, 0.0 };
};
struct AABB
{
	AABB() = default;
	AABB(const glm::dvec3& min, const glm::dvec3& max)
		: m_min(min), m_max(max) {}

	glm::dvec3 m_min = { 0.0, 0.0, 0.0 },
		m_max = { 0.0, 0.0, 0.0 };
};

struct TraceInfo
{
	bool m_hit;
	double m_fraction;

	glm::dvec3 m_normal,
		m_trace_start;
};

class World;
class RayTracer
{
public:
	bool Trace(const Ray& ray, TraceInfo& trace_info);

	void SetWorld(World* world) { m_world = world; }

private:
	static bool RaytoPlaneIntersection(const Ray& ray, const Plane& plane, double* intersection_distance = nullptr);
	static bool RaytoAABBIntersection(const Ray& ray, const AABB& aabb, double* fraction = nullptr, glm::dvec3* intersection_normal = nullptr);
	static bool AABBtoAABBIntersection(const Ray& ray, const AABB& aabb, double* fraction = nullptr, glm::dvec3* intersection_normal = nullptr);

private:
	World* m_world = nullptr;
};