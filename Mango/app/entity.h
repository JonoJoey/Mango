#pragma once

#include "../mango/graphics/3D/entity_3d.h"


class World;

class Entity : public Mango::Entity3D
{
public:
	virtual void Render() {}
	virtual void OnUpdate() {}
	virtual void OnInit() {}
	virtual void OnRelease() {}

	void SetWorld(World* world) { m_world = world; }
private:
	World* m_world = nullptr;
};