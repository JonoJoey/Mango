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
	glm::vec3 start;
	glm::vec3 direction;
	float length;
	float step;
};

struct TraceInfo
{
	Entity* hit_entity = nullptr;

	BLOCK_FACE block_face;
	bool did_hit_block = false;

	glm::vec3 trace_end;

};

class RayTracer
{
public:
	bool Trace(Ray ray, TraceInfo& trace_info);

	void SetWorld(World* world) { m_world = world; }
private:
	World* m_world = nullptr;
};