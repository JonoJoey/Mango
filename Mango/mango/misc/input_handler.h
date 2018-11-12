#pragma once

#include <string>



namespace Mango
{
	enum INPUT_STATE
	{
		INPUT_STATE_RELEASE,
		INPUT_STATE_PRESS,
		INPUT_STATE_REPEAT
	};

	// input interface
	// TODO: custom keycode mapping
	class IInputHandler
	{
	public:
		virtual void OnKeyPress(int key, std::string key_name, bool repeat) {};
		virtual void OnKeyRelease(int key, std::string key_name) {};
		virtual void OnMouseMove(float xpos, float ypos) {};
		virtual void OnMouseButtonPress(int button, bool repeat) {};
		virtual void OnMouseButtonRelease(int button) {};

	private:
	};
} // namespace Mango