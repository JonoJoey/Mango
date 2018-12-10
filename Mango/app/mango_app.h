#pragma once

#include "../mango/mango.h"

#include "event_handler.h"
#include "world.h"
#include "block_map.h"


class LocalPlayer;


class MangoApp
{
public:
	void Run();

	Mango::MangoCore* GetMangoCore() { return &m_mango_core; }
	Mango::InputHandler* GetInputHandler() { return &m_input_handler; }
	Mango::Framebuffer* GetFramebuffer() { return &m_framebuffer; }
	World* GetWorld() { return &m_world; }
	unsigned int GetTickCount() const { return m_tick_count; }
	double GetTickInterval() const { return m_interval_per_tick; }

private:
	bool OnInit();
	void OnRelease();

	void OnTick();
	void OnFrame(double frame_time, double lerp);

private:
	Mango::MangoCore m_mango_core;
	Mango::InputHandler m_input_handler;
	Mango::Framebuffer m_framebuffer;
	EventHandler m_event_handler;

	const double m_interval_per_tick = 1.0 / 256.0;
	unsigned int m_tick_count = 0;
	double m_curtime = 0.0;

	World m_world;
	LocalPlayer* m_local_player = nullptr;
};