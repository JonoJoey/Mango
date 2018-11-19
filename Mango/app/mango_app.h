#pragma once

#include "../mango/mango.h"

#include "event_handler.h"
#include "components.h"


class MangoApp
{
public:
	void Run();

	Mango::MangoCore* GetMangoCore() { return &m_mango_core; }
	Mango::ECS::ECSCore* GetECSCore() { return &m_ecs_core; }
	Mango::Framebuffer* GetFramebuffer() { return &m_framebuffer; }

private:
	Mango::MangoCore m_mango_core;
	Mango::ECS::ECSCore m_ecs_core;
	EventHandler m_event_handler;

	Mango::Framebuffer m_framebuffer;
};