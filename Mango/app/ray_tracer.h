#pragma once

#include "block.h"


class World;
class Entity;

enum BLOCK_FACE
{
	BLOCK_FACE_FRONT,
	BLOCK_FACE_BACK,
	BLOCK_FACE_RIGHT,
	BLOCK_FACE_LEFT,
	BLOCK_FACE_TOP,
	BLOCK_FACE_BOTTOM,

	NUM_BLOCK_FACES
};

struct Ray
{
	Ray() = default;
	Ray(glm::vec3 start, glm::vec3 direction, float length, float step)
		: m_start(start), m_direction(direction), m_length(length), m_step(step) {}
	Ray(glm::vec3 start, glm::vec3 end, float step)
		: m_start(start), m_direction(end - start), m_length(glm::length(end - start)), m_step(step) {}

	glm::vec3 m_start;
	glm::vec3 m_direction;
	float m_length;
	float m_step;
};

struct TraceInfo
{
	Entity* hit_entity = nullptr;

	BLOCK_FACE block_face;
	bool did_hit_block = false;

	glm::vec3 trace_end;
	float fraction = 0.f;

};

class RayTracer
{
public:
	bool Trace(Ray ray, TraceInfo& trace_info);

	void SetWorld(World* world) { m_world = world; }
private:
	World* m_world = nullptr;
};