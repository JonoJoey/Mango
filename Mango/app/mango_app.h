#pragma once

#include "../mango/mango.h"

#include "event_handler.h"


class MangoApp
{
public:
	void Run();

	Mango::MangoCore* GetMangoCore() { return &m_mango_core; }
	Mango::InputHandler* GetInputHandler() { return &m_input_handler; }
	Mango::Framebuffer* GetFramebuffer() { return &m_framebuffer; }

private:
	void OnInit();
	void OnTick();
	void OnFrame(float lerptime);

private:
	Mango::MangoCore m_mango_core;
	Mango::InputHandler m_input_handler;
	Mango::Framebuffer m_framebuffer;
	EventHandler m_event_handler;

	const float m_interval_per_tick = 1.f / 60.f;
};