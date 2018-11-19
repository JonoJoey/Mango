#pragma once

#include "../mango/mango.h"


NEW_COMPONENT(PositionComponent)
{
	PositionComponent() = default;
	PositionComponent(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
	float x, y, z;
}; COMPONENT_INFO(PositionComponent, "position");

NEW_COMPONENT(VelocityComponent)
{
	VelocityComponent() = default;
	VelocityComponent(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
	float x, y, z;
}; COMPONENT_INFO(VelocityComponent, "velocity");