#include "ray_tracer.h"

#include "world.h"


bool RayTracer::Trace(Ray ray, TraceInfo& trace_info)
{
	static const glm::vec3 normals[NUM_BLOCK_FACES] =
	{
		{ 0.f, 0.f, 1.f },
		{ 0.f, 0.f, -1.f },
		{ 1.f, 0.f, 0.f },
		{ -1.f, 0.f, 0.f },
		{ 0.f, 1.f, 0.f },
		{ 0.f, -1.f, 0.f }
	};

	// ray -> plane intersection
	const auto IntersectPlane = [](const glm::vec3& ray_origin, const glm::vec3& ray_direction, 
		const glm::vec3& plane_center, const glm::vec3& plane_normal, glm::vec3* intersect_point = nullptr) -> bool
	{
		const float denom = glm::dot(plane_normal, ray_direction);
		if (fabs(denom) > 0.f)
		{
			const float distance = glm::dot(plane_center - ray_origin, plane_normal) / denom;
			if (distance >= 0.f)
			{
				if (intersect_point)
					*intersect_point = ray_origin + ray_direction * distance;

				return true;
			}
		}

		return false;
	};

	glm::vec3 position = ray.m_start;
	glm::vec3 direction = glm::normalize(ray.m_direction);

	float distance = 0.f;
	while (distance <= ray.m_length)
	{
		Block block;
		if (m_world->GetBlock(int(floorf(position.x)), int(floorf(position.y)), int(floorf(position.z)), block) && block.m_is_active)
		{
			glm::vec3 face_points[NUM_BLOCK_FACES];
			const auto block_center = glm::vec3(floorf(position.x) + 0.5f, floorf(position.y) + 0.5f, floorf(position.z) + 0.5f);

			// calculate intersection points for each face
			for (int i = 0; i < NUM_BLOCK_FACES; i++)
			{
				if (glm::vec3 int_point; IntersectPlane(ray.m_start, direction, block_center + (normals[i] * 0.5f), normals[i], &int_point) &&
					fabs(int_point.x - block_center.x) <= 0.5f && fabs(int_point.y - block_center.y) <= 0.5f && fabs(int_point.z - block_center.z) <= 0.5f)
					face_points[i] = int_point;
				else
					face_points[i] = { FLT_MAX, FLT_MAX, FLT_MAX };
			}

			// calculate the closest face
			BLOCK_FACE closest_face = NUM_BLOCK_FACES;
			float closest_face_dist = FLT_MAX;
			for (int i = 0; i < NUM_BLOCK_FACES; i++)
			{
				const float dist = glm::length(face_points[i] - ray.m_start);
				if (dist < closest_face_dist)
				{
					closest_face_dist = dist;
					closest_face = BLOCK_FACE(i);
				}
			}

			if (closest_face_dist <= distance)
				trace_info.trace_end = face_points[closest_face];
			else
				trace_info.trace_end = position;

			trace_info.fraction = glm::length(trace_info.trace_end - ray.m_start) / ray.m_length;
			trace_info.block_face = closest_face;
			trace_info.did_hit_block = true;
			return true;
		}

		position += direction * ray.m_step;
		distance += ray.m_step;
	}

	trace_info.fraction = 1.f;
	return false;
}