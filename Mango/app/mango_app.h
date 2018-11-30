#pragma once

#include "../mango/mango.h"

#include "event_handler.h"

#include "world.h"


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
	float GetTickInterval() const { return m_interval_per_tick; }
	const std::unordered_map<std::string, BLOCK_ID>& GetBlockMap() const { return m_block_map; }

private:
	void OnInit();
	void OnRelease();

	void OnTick();
	void OnFrame(float frame_time, float lerp);

private:
	Mango::MangoCore m_mango_core;
	Mango::InputHandler m_input_handler;
	Mango::Framebuffer m_framebuffer;
	EventHandler m_event_handler;

	const float m_interval_per_tick = 1.f / 32.f;
	unsigned int m_tick_count = 0;
	float m_mouse_sensitivity = 0.5f;

	std::unordered_map<std::string, BLOCK_ID> m_block_map;
	World m_world;
	LocalPlayer* m_local_player = nullptr;
};