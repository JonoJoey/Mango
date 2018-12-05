#include "ray_tracer.h"

#include "world.h"

#include <limits>


bool RayTracer::Trace(const Ray& ray, TraceInfo& trace_info)
{
	trace_info.m_hit = false;
	trace_info.m_normal = { 0.0, 0.0, 0.0 };
	trace_info.m_trace_start = ray.m_start;
	trace_info.m_fraction = 0.0;

	if (glm::length(ray.m_direction) <= 0.0 || ray.m_length < 0.0)
		return false;

	// how much to step in ray.m_direction each iteration (MAYBE DYNAMICALLY CHANGE THIS)
	static constexpr double STEP_VALUE = 0.001;

	for (double current_dist(0.0); current_dist <= ray.m_length; current_dist += STEP_VALUE)
	{
		if (ray.m_is_ray)
		{
			const glm::dvec3 current_pos(ray.m_start + ray.m_direction * current_dist);
			const glm::dvec3 block_pos(std::floor(current_pos.x), std::floor(current_pos.y), std::floor(current_pos.z));
			if (Block block; !m_world->GetBlock(block_pos.x, block_pos.y, block_pos.z, block) || !block.m_is_active)
				continue;

			glm::dvec3 int_normal(FLT_MAX);
			double int_fraction(FLT_MAX);
			if (!RaytoAABBIntersection(ray, AABB(block_pos, block_pos + 1.0), &int_fraction, &int_normal))
				continue;

			trace_info.m_hit = true;
			trace_info.m_normal = int_normal;
			trace_info.m_trace_start = ray.m_start;
			trace_info.m_fraction = int_fraction;
			return true;
		}
		else
		{
			std::deque<AABB> aabbs;

			const auto BananaMango = [this, &aabbs, &ray, current_dist](glm::dvec3 start) -> void
			{
				const glm::dvec3 current_pos(start + ray.m_direction * current_dist);
				const glm::dvec3 block_pos(std::floor(current_pos.x), std::floor(current_pos.y), std::floor(current_pos.z));
				if (Block block; m_world->GetBlock(block_pos.x, block_pos.y, block_pos.z, block) && block.m_is_active)
					aabbs.emplace_back(block_pos, block_pos + 1.0);
			};
			BananaMango(ray.m_start + glm::dvec3(ray.m_extents.x, ray.m_extents.y, ray.m_extents.z));
			BananaMango(ray.m_start + glm::dvec3(-ray.m_extents.x, ray.m_extents.y, ray.m_extents.z));
			BananaMango(ray.m_start + glm::dvec3(-ray.m_extents.x, -ray.m_extents.y, ray.m_extents.z));
			BananaMango(ray.m_start + glm::dvec3(-ray.m_extents.x, ray.m_extents.y, -ray.m_extents.z));
			BananaMango(ray.m_start - glm::dvec3(ray.m_extents.x, ray.m_extents.y, ray.m_extents.z));
			BananaMango(ray.m_start - glm::dvec3(-ray.m_extents.x, ray.m_extents.y, ray.m_extents.z));
			BananaMango(ray.m_start - glm::dvec3(-ray.m_extents.x, -ray.m_extents.y, ray.m_extents.z));
			BananaMango(ray.m_start - glm::dvec3(-ray.m_extents.x, ray.m_extents.y, -ray.m_extents.z));
			if (aabbs.empty())
				continue;

			glm::dvec3 closest_normal;
			double closest_fraction = 1.0;
			for (const auto& aabb : aabbs)
			{
				double fraction; glm::dvec3 normal;
				if (!AABBtoAABBIntersection(ray, aabb, &fraction, &normal))
					continue;

				if (fraction < closest_fraction)
				{
					closest_fraction = fraction;
					closest_normal = normal;
				}
			}

			if (closest_fraction >= 1.0)
				break;

			trace_info.m_hit = true;
			trace_info.m_normal = closest_normal;
			trace_info.m_trace_start = ray.m_start;
			trace_info.m_fraction = closest_fraction;
			return true;
		}
	}

	trace_info.m_fraction = 1.0;
	return false;
}

bool RayTracer::RaytoPlaneIntersection(const Ray& ray, const Plane& plane, double* fraction)
{
	const auto denom = glm::dot(plane.m_normal, ray.m_direction);
	if (std::abs(denom) <= 0.00001)
		return false;

	const auto distance = glm::dot(plane.m_point - ray.m_start, plane.m_normal) / denom;
	if (distance < 0.0 || distance > ray.m_length)
		return false;

	if (fraction)
		*fraction = ray.m_length > 0.0 ? distance / ray.m_length : 1.0;

	return true;
};
bool RayTracer::RaytoAABBIntersection(const Ray& ray, const AABB& aabb, double* fraction, glm::dvec3* intersection_normal)
{
	const auto aabb_center = (aabb.m_min + aabb.m_max) * 0.5;
	const auto extents = (aabb.m_max - aabb.m_min) * 0.5;
	const auto velocity = ray.m_direction * ray.m_length;

	glm::dvec3 in_dist, out_dist;
	for (int i = 0; i < 3; i++)
	{
		if (velocity[i] > 0.0)
		{
			in_dist[i] = ((aabb_center[i] - extents[i]) - ray.m_start[i]);
			out_dist[i] = ((aabb_center[i] + extents[i]) - ray.m_start[i]);
		}
		else
		{
			in_dist[i] = ((aabb_center[i] + extents[i]) - ray.m_start[i]);
			out_dist[i] = ((aabb_center[i] - extents[i]) - ray.m_start[i]);
		}
	}

	glm::dvec3 in_frac, out_frac;
	for (int i = 0; i < 3; i++)
	{
		if (velocity[i] == 0.0)
			in_frac[i] = out_frac[i] = 0.0;
		else
		{
			in_frac[i] = in_dist[i] / velocity[i];
			out_frac[i] = out_dist[i] / velocity[i];
		}
	}

	const auto enter_fraction = std::max(std::max(in_frac[0], in_frac[1]), in_frac[2]);
	const auto exit_fraction = std::min(std::min(out_frac[0], out_frac[1]), out_frac[2]);
	if (enter_fraction > exit_fraction || enter_fraction < 0.0 || enter_fraction >= 1.0)
		return false;

	if (fraction)
		*fraction = enter_fraction;

	if (intersection_normal)
	{
		*intersection_normal = glm::dvec3(0.0);

		if (in_frac[0] > std::max(in_frac[1], in_frac[2]))
		{
			if (in_dist[0] >= 0.0)
				(*intersection_normal)[0] = -1.0;
			else
				(*intersection_normal)[0] = 1.0;
		}
		else if (in_frac[1] > std::max(in_frac[0], in_frac[2]))
		{
			if (in_dist[1] >= 0.0)
				(*intersection_normal)[1] = -1.0;
			else
				(*intersection_normal)[1] = 1.0;
		}
		else
		{
			if (in_dist[2] >= 0.0)
				(*intersection_normal)[2] = -1.0;
			else
				(*intersection_normal)[2] = 1.0;
		}
	}

	return true;
}
bool RayTracer::AABBtoAABBIntersection(const Ray& ray, const AABB& aabb, double* fraction, glm::dvec3* intersection_normal)
{
	const auto aabb_center = (aabb.m_min + aabb.m_max) * 0.5;
	const auto extents = (aabb.m_max - aabb.m_min) * 0.5;
	const auto velocity = ray.m_direction * ray.m_length;

	glm::dvec3 in_dist, out_dist;
	for (int i = 0; i < 3; i++)
	{
		if (velocity[i] >= 0.0)
		{
			in_dist[i] = ((aabb_center[i] - extents[i]) - (ray.m_start[i] + ray.m_extents[i]));
			out_dist[i] = ((aabb_center[i] + extents[i]) - (ray.m_start[i] - ray.m_extents[i]));
		}
		else
		{
			in_dist[i] = ((aabb_center[i] + extents[i]) - (ray.m_start[i] - ray.m_extents[i]));
			out_dist[i] = ((aabb_center[i] - extents[i]) - (ray.m_start[i] + ray.m_extents[i]));
		}
	}

	glm::dvec3 in_frac, out_frac;
	for (int i = 0; i < 3; i++)
	{
		if (velocity[i] == 0.0)
		{
			in_frac[i] = -std::numeric_limits<double>::infinity();
			out_frac[i] = std::numeric_limits<double>::infinity();
		}
		else
		{
			in_frac[i] = in_dist[i] / velocity[i];
			out_frac[i] = out_dist[i] / velocity[i];
		}
	}

	const auto enter_fraction = std::max(std::max(in_frac[0], in_frac[1]), in_frac[2]);
	const auto exit_fraction = std::min(std::min(out_frac[0], out_frac[1]), out_frac[2]);
	if (enter_fraction > exit_fraction || enter_fraction < 0.0 || enter_fraction > 1.0)
		return false;

	if (fraction)
		*fraction = enter_fraction;

	if (intersection_normal)
	{
		*intersection_normal = glm::dvec3(0.0);

		if (in_frac[0] > std::max(in_frac[1], in_frac[2]))
		{
			if (in_dist[0] >= 0.0)
				(*intersection_normal)[0] = -1.0;
			else
				(*intersection_normal)[0] = 1.0;
		}
		else if (in_frac[1] > std::max(in_frac[0], in_frac[2]))
		{
			if (in_dist[1] >= 0.0)
				(*intersection_normal)[1] = -1.0;
			else
				(*intersection_normal)[1] = 1.0;
		}
		else
		{
			if (in_dist[2] >= 0.0)
				(*intersection_normal)[2] = -1.0;
			else
				(*intersection_normal)[2] = 1.0;
		}

		if (glm::dot(*intersection_normal, ray.m_direction) == 0.0)
			DBG_LOG("dab");
	}

	return true;
}