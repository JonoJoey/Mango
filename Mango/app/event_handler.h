#pragma once

#include "../mango/mango.h"


class MangoApp;
class EventHandler : public Mango::IEventHandler
{
public:
	void OnKeyPress(int key, std::string key_name, bool repeat);
	void OnKeyRelease(int key, std::string key_name);

	void OnMouseMove(float xpos, float ypos);
	void OnMouseButtonPress(int button, bool repeat);
	void OnMouseButtonRelease(int button);

	void OnWindowResize(int width, int height);

	void SetApp(MangoApp* app) { m_mango_app = app; }

private:
	MangoApp* m_mango_app = nullptr;
};