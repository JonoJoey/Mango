#pragma once

#include <string>



namespace Mango
{
	class MangoCore;

	enum INPUT_STATE
	{
		INPUT_STATE_RELEASE,
		INPUT_STATE_PRESS,
		INPUT_STATE_REPEAT
	};

	// event interface (mostly input)
	// TODO: custom keycode mapping
	class IEventHandler
	{
	public:
		virtual void OnKeyPress(int key, std::string key_name, bool repeat) {};
		virtual void OnKeyRelease(int key, std::string key_name) {};

		virtual void OnMouseMove(float xpos, float ypos) {};
		virtual void OnMouseButtonPress(int button, bool repeat) {};
		virtual void OnMouseButtonRelease(int button) {};

		virtual void OnWindowResize(int width, int height) {};
	};
} // namespace Mango