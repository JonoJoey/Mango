#include "ray_tracer.h"

#include "world.h"



bool RayTracer::Trace(const Ray& ray, TraceInfo& trace_info)
{
	// how much to step in ray.m_direction each iteration (MAYBE DYNAMICALLY CHANGE THIS)
	static constexpr double STEP_VALUE = 0.001;

	for (double current_dist = 0.0; current_dist <= ray.m_length; current_dist += STEP_VALUE)
	{
		const glm::dvec3 current_pos(ray.m_start + ray.m_direction * current_dist);
		const glm::dvec3 block_pos(std::floor(current_pos.x), std::floor(current_pos.y), std::floor(current_pos.z));

		if (Block block; !m_world->GetBlock(block_pos.x, block_pos.y, block_pos.z, block) || !block.m_is_active)
			continue;

		AABB aabb(block_pos, block_pos + 1.0);
		glm::dvec3 int_normal;
		double int_dist;

		if (!RayAABBIntersection(ray, aabb, &int_dist, &int_normal))
			break;

		trace_info.m_hit = true;
		trace_info.m_normal = int_normal;
		trace_info.m_trace_start = ray.m_start;
		trace_info.m_trace_end = ray.m_start + ray.m_direction * int_dist;
		trace_info.m_fraction = int_dist / ray.m_length;
		return true;
	}

	trace_info.m_hit = false;
	trace_info.m_normal = { 0.0, 0.0, 0.0 };
	trace_info.m_trace_start = ray.m_start;
	trace_info.m_trace_end = ray.m_start + ray.m_direction * ray.m_length;
	trace_info.m_fraction = 1.0;
	return false;
}
bool RayTracer::RayPlaneIntersection(const Ray& ray, const Plane& plane, double* intersection_distance)
{
	const auto denom = glm::dot(plane.m_normal, ray.m_direction);
	if (std::abs(denom) <= 0.0)
		return false;

	const auto distance = glm::dot(plane.m_point - ray.m_start, plane.m_normal) / denom;
	if (distance < 0.0 || distance > ray.m_length)
		return false;

	if (intersection_distance)
		*intersection_distance = distance;

	return true;
};
bool RayTracer::RayAABBIntersection(const Ray& ray, const AABB& aabb, double* intersection_distance, glm::dvec3* intersection_normal)
{
	const auto aabb_center = (aabb.m_min + aabb.m_max) * 0.5;
	
	glm::dvec3 normal;
	double closest_distance = FLT_MAX;
	for (int i = 0; i < 3; i++)
	{
		Plane plane_1(aabb_center, { 0.0, 0.0, 0.0 });
		plane_1.m_point[i] = aabb.m_max[i];
		plane_1.m_normal[i] = 1.0;

		Plane plane_2(aabb_center, { 0.0, 0.0, 0.0 });
		plane_2.m_point[i] = aabb.m_min[i];
		plane_2.m_normal[i] = -1.0;
		
		if (double distance; RayPlaneIntersection(ray, plane_1, &distance) && distance <= closest_distance)
		{
			const auto int_point = ray.m_start + ray.m_direction * distance;
			if (std::abs(int_point.x - aabb_center.x) <= (aabb.m_max.x - aabb.m_min.x) * 0.5 &&
				std::abs(int_point.y - aabb_center.y) <= (aabb.m_max.y - aabb.m_min.y) * 0.5 &&
				std::abs(int_point.z - aabb_center.z) <= (aabb.m_max.z - aabb.m_min.z) * 0.5)
			{
				if (distance == closest_distance)
					DBG_LOG("CORNER? - (%f, %f, %f) - (%f)", plane_1.m_normal.x, plane_1.m_normal.y, plane_1.m_normal.z, distance);

				closest_distance = distance;
				normal = plane_1.m_normal;
			}
		}

		if (double distance; RayPlaneIntersection(ray, plane_2, &distance) && distance <= closest_distance)
		{
			const auto int_point = ray.m_start + ray.m_direction * distance;
			if (std::abs(int_point.x - aabb_center.x) <= (aabb.m_max.x - aabb.m_min.x) * 0.5 &&
				std::abs(int_point.y - aabb_center.y) <= (aabb.m_max.y - aabb.m_min.y) * 0.5 &&
				std::abs(int_point.z - aabb_center.z) <= (aabb.m_max.z - aabb.m_min.z) * 0.5)
			{
				if (distance == closest_distance)
					DBG_LOG("CORNER? - (%f, %f, %f) - (%f)", plane_2.m_normal.x, plane_2.m_normal.y, plane_2.m_normal.z, distance);

				closest_distance = distance;
				normal = plane_2.m_normal;
			}
		}
	}

	if (closest_distance == FLT_MAX)
		return false;

	if (intersection_distance)
		*intersection_distance = closest_distance;

	if (intersection_normal)
		*intersection_normal = normal;

	return true;
}

//bool RayTracer::Trace(Ray ray, TraceInfo& trace_info)
//{
//	static const glm::vec3 normals[NUM_BLOCK_FACES] =
//	{
//		{ 0.f, 0.f, 1.f },
//		{ 0.f, 0.f, -1.f },
//		{ 1.f, 0.f, 0.f },
//		{ -1.f, 0.f, 0.f },
//		{ 0.f, 1.f, 0.f },
//		{ 0.f, -1.f, 0.f }
//	};
//
//	// ray -> plane intersection
//	const auto IntersectPlane = [](const glm::vec3& ray_origin, const glm::vec3& ray_direction, 
//		const glm::vec3& plane_center, const glm::vec3& plane_normal, glm::vec3* intersect_point = nullptr) -> bool
//	{
//		const float denom = glm::dot(plane_normal, ray_direction);
//		if (fabs(denom) > 0.f)
//		{
//			const float distance = glm::dot(plane_center - ray_origin, plane_normal) / denom;
//			if (distance >= 0.f)
//			{
//				if (intersect_point)
//					*intersect_point = ray_origin + ray_direction * distance;
//
//				return true;
//			}
//		}
//
//		return false;
//	};
//
//	ASSERT(m_world);
//
//	trace_info.trace_start = ray.m_start;
//
//	glm::vec3 position = ray.m_start;
//	glm::vec3 direction = glm::normalize(ray.m_direction);
//
//	float distance = 0.f;
//	while (distance <= ray.m_length)
//	{
//		Block block;
//		if (m_world->GetBlock(int(floorf(position.x)), int(floorf(position.y)), int(floorf(position.z)), block) && block.m_is_active)
//		{
//			glm::vec3 face_points[NUM_BLOCK_FACES];
//			const auto block_center = glm::vec3(floorf(position.x) + 0.5f, floorf(position.y) + 0.5f, floorf(position.z) + 0.5f);
//
//			// calculate intersection points for each face
//			for (int i = 0; i < NUM_BLOCK_FACES; i++)
//			{
//				if (glm::vec3 int_point; IntersectPlane(ray.m_start, direction, block_center + (normals[i] * 0.5f), normals[i], &int_point) &&
//					fabs(int_point.x - block_center.x) <= 0.5f && fabs(int_point.y - block_center.y) <= 0.5f && fabs(int_point.z - block_center.z) <= 0.5f)
//					face_points[i] = int_point;
//				else
//					face_points[i] = { FLT_MAX, FLT_MAX, FLT_MAX };
//			}
//
//			// calculate the closest face
//			BLOCK_FACE closest_face = NUM_BLOCK_FACES;
//			float closest_face_dist = ray.m_length;
//			for (int i = 0; i < NUM_BLOCK_FACES; i++)
//			{
//				if (face_points[i].x == FLT_MAX || face_points[i].y == FLT_MAX || face_points[i].z == FLT_MAX)
//					continue;
//
//				const float dist = glm::length(face_points[i] - ray.m_start);
//				if (dist < closest_face_dist)
//				{
//					closest_face_dist = dist;
//					closest_face = BLOCK_FACE(i);
//				}
//			}
//
//			if (closest_face == NUM_BLOCK_FACES)
//			{
//				position += direction * ray.m_step;
//				distance += ray.m_step;
//				continue;
//			}
//
//			trace_info.trace_end = face_points[closest_face];
//			trace_info.normal = normals[closest_face];
//			trace_info.start_solid = (distance <= 0.f);
//			trace_info.fraction = (ray.m_length > 0.f) ? (glm::length(trace_info.trace_end - ray.m_start) / ray.m_length) : 0.f;
//			trace_info.block_face = closest_face;
//			trace_info.did_hit_block = true;
//
//			return true;
//		}
//
//		position += direction * ray.m_step;
//		distance += ray.m_step;
//	}
//
//	trace_info.trace_end = trace_info.trace_start + direction * ray.m_length;
//	trace_info.fraction = 1.f;
//	return false;
//}