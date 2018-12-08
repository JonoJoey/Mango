#include "ray_tracer.h"

#include "world.h"

#include <limits>


//struct NEWAABB
//{
//	NEWAABB() = default;
//	NEWAABB(const glm::dvec3& center, const glm::dvec3& extent)
//		: m_center(center), m_extent(extent) {}
//
//	glm::dvec3 m_center = { 0.0, 0.0, 0.0 },
//		m_extent = { 0.0, 0.0, 0.0 };
//};


static bool AABBtoAABBSweep(const NEWAABB& aabb1, const NEWAABB& aabb2, const glm::dvec3& velocity, double& near_frac, double& far_frac, glm::dvec3& normal)
{
	glm::dvec3 near_offset(0.0), far_offset(0.0);
	for (int i = 0; i < 3; i++)
	{
		if (velocity[i] >= 0.0)
		{
			near_offset[i] = (aabb2.m_center[i] - aabb2.m_extent[i]) - (aabb1.m_center[i] + aabb1.m_extent[i]);
			far_offset[i] = (aabb2.m_center[i] + aabb2.m_extent[i]) - (aabb1.m_center[i] - aabb1.m_extent[i]);
		}
		else
		{
			near_offset[i] = (aabb1.m_center[i] - aabb1.m_extent[i]) - (aabb2.m_center[i] + aabb2.m_extent[i]);
			far_offset[i] = (aabb1.m_center[i] + aabb1.m_extent[i]) - (aabb2.m_center[i] - aabb2.m_extent[i]);
		}
	}

	glm::dvec3 near_fraction(0.0), far_fraction(0.0);
	for (int i = 0; i < 3; i++)
	{
		if (velocity[i] == 0.0)
		{
			const auto diff = aabb1.m_center[i] - aabb2.m_center[i];
			if (std::abs(diff) >= aabb1.m_extent[i] + aabb2.m_extent[i])
			{
				if (std::abs(diff) == aabb1.m_extent[i] + aabb2.m_extent[i])
				{
					glm::dvec3 edge_normal(0.0);
					edge_normal[i] = diff >= 0.0 ? 1.0 : -1.0;
					//DBG_LOG("%f %f %f", edge_normal[0], edge_normal[1], edge_normal[2]);
					if (glm::dot(edge_normal, glm::safe_normalize(velocity)) == 0.0)
						return false;
				}
				else
					return false;
			}

			near_fraction[i] = -std::numeric_limits<double>::infinity();
			far_fraction[i] = std::numeric_limits<double>::infinity();
		}
		else
		{
			near_fraction[i] = near_offset[i] / std::abs(velocity[i]);
			far_fraction[i] = far_offset[i] / std::abs(velocity[i]);
		}
	}

	near_frac = std::max(std::max(near_fraction[0], near_fraction[1]), near_fraction[2]);
	far_frac = std::min(std::min(far_fraction[0], far_fraction[1]), far_fraction[2]);
	if (near_frac > far_frac || near_frac < 0.0 || near_frac >= 1.0)
		return false;

	// compute collision normal
	{
		normal = glm::dvec3(0.0);

		if (near_fraction[0] >= std::max(near_fraction[1], near_fraction[2]))
		{
			if (velocity[0] >= 0.0)
				normal[0] = -1.0;
			else
				normal[0] = 1.0;
		}
		if (near_fraction[1] >= std::max(near_fraction[0], near_fraction[2]))
		{
			if (velocity[1] >= 0.0)
				normal[1] = -1.0;
			else
				normal[1] = 1.0;
		}
		if (near_fraction[2] >= std::max(near_fraction[0], near_fraction[1]))
		{
			if (velocity[2] >= 0.0)
				normal[2] = -1.0;
			else
				normal[2] = 1.0;
		}

		normal = glm::safe_normalize(normal);
		if (glm::dot(normal, glm::safe_normalize(velocity)) == 0.0)
			return false;
	}

	return true;
}


void RayTracer::Test()
{
	const auto DoTest = [](const std::string& test_name, const NEWAABB& aabb1, const NEWAABB& aabb2, const glm::dvec3& velocity) -> void
	{
		double in_frac, out_frac;
		glm::dvec3 normal;
		if (!AABBtoAABBSweep(aabb1, aabb2, velocity, in_frac, out_frac, normal))
			DBG_LOG("%s: Did not intersect", test_name.c_str());
		else
			DBG_LOG("%s: \n  in_frac:%f \n  out_frac:%f \n  normal:(%f, %f, %f)", test_name.c_str(), in_frac, out_frac, normal[0], normal[1], normal[2]);
	};

	DoTest("test 1", 
		NEWAABB({ 0.0, 0.0, 0.0 }, glm::dvec3(0.5)), 
		NEWAABB({ 5.0, 5.0, 0.0 }, glm::dvec3(0.5)), 
		{ 10.0, 0.0, 0.0 });
	DoTest("test 2",
		NEWAABB({ 0.0, 1.1, 0.0 }, glm::dvec3(0.5)),
		NEWAABB({ 5.0, 0.0, 0.0 }, glm::dvec3(0.5)),
		{ 10.0, 0.0, 0.0 });

	DoTest("test 3",
		NEWAABB({ 5.0, 0.0, 0.0 }, glm::dvec3(0.5)),
		NEWAABB({ 0.0, -1.1, 0.0 }, glm::dvec3(0.5)),
		{ -10.0, 0.0, 0.0 });
	DoTest("test 4",
		NEWAABB({ 5.0, 1.1, 0.0 }, glm::dvec3(0.5)),
		NEWAABB({ 0.0, 0.2, 0.0 }, glm::dvec3(0.5)),
		{ -10.0, 0.0, 0.0 });
}
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
			if (!RaytoAABBIntersection(ray, NEWAABB(block_pos + 0.5, glm::dvec3(0.5)), &int_fraction, &int_normal))
				continue;

			trace_info.m_hit = true;
			trace_info.m_normal = int_normal;
			trace_info.m_trace_start = ray.m_start;
			trace_info.m_fraction = int_fraction;
			return true;
		}
		else
		{
			std::deque<NEWAABB> aabbs;

			const auto BananaMango = [this, &aabbs, &ray, current_dist](glm::dvec3 start) -> void
			{
				const glm::dvec3 current_pos(start + ray.m_direction * current_dist);
				const glm::dvec3 block_pos(std::floor(current_pos.x), std::floor(current_pos.y), std::floor(current_pos.z));
				if (Block block; m_world->GetBlock(block_pos.x, block_pos.y, block_pos.z, block) && block.m_is_active)
					aabbs.emplace_back(block_pos + 0.5, glm::dvec3(0.5));
			};
			BananaMango(ray.m_start);
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

					//DBG_LOG("%f %f", fraction, glm::length(ray.m_start - aabb.m_center));
				}
			}

			if (closest_fraction >= 1.0)
				continue;

			trace_info.m_hit = true;
			trace_info.m_normal = closest_normal;
			trace_info.m_trace_start = ray.m_start;
			trace_info.m_fraction = closest_fraction;
			return true;
		}
	}

	trace_info.m_hit = false;
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
bool RayTracer::RaytoAABBIntersection(const Ray& ray, const NEWAABB& aabb, double* fraction, glm::dvec3* intersection_normal)
{
	double near_frac, far_frac; glm::dvec3 normal;
	const bool ret = AABBtoAABBSweep(NEWAABB(ray.m_start, { 0.0, 0.0, 0.0 }), aabb, ray.m_direction * ray.m_length, near_frac, far_frac, normal);

	if (fraction)
		*fraction = near_frac;
	if (intersection_normal)
		*intersection_normal = normal;

	return ret;
}
bool RayTracer::AABBtoAABBIntersection(const Ray& ray, const NEWAABB& aabb, double* fraction, glm::dvec3* intersection_normal)
{
	double near_frac, far_frac; glm::dvec3 normal;
	const bool ret = AABBtoAABBSweep(NEWAABB(ray.m_start, ray.m_extents), aabb, ray.m_direction * ray.m_length, near_frac, far_frac, normal);

	if (fraction)
		*fraction = near_frac;
	if (intersection_normal)
		*intersection_normal = normal;

	return ret;
}