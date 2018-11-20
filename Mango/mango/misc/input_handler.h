#pragma once

#include "../common.h"

#include "event_handler.h"


namespace Mango
{
	class InputHandler : public IEventHandler
	{
	public:
		void OnKeyPress(int key, std::string key_name, bool repeat) 
		{
			m_key_states[key] = repeat ? INPUT_STATE::INPUT_STATE_REPEAT : INPUT_STATE::INPUT_STATE_PRESS;
		}
		void OnKeyRelease(int key, std::string key_name) 
		{
			m_key_states[key] = INPUT_STATE::INPUT_STATE_RELEASE;
		}

		void OnMouseButtonPress(int button, bool repeat) 
		{
			m_button_states[button] = repeat ? INPUT_STATE::INPUT_STATE_REPEAT : INPUT_STATE::INPUT_STATE_PRESS;
		}
		void OnMouseButtonRelease(int button) 
		{
			m_button_states[button] = INPUT_STATE::INPUT_STATE_RELEASE;
		}

		void OnMouseMove(float xpos, float ypos)
		{
			m_mouse_pos = { xpos, ypos };
		}

		glm::vec2 GetMousePosition()
		{
			return m_mouse_pos;
		}
		INPUT_STATE GetKeyState(int key)
		{
			if (auto pair = m_key_states.find(key); pair != m_key_states.end())
			{
				const auto ret = pair->second;
				if (ret)
					pair->second = INPUT_STATE::INPUT_STATE_REPEAT;
				return ret;
			}
			else
				return m_key_states[key] = INPUT_STATE::INPUT_STATE_RELEASE;
		}
		INPUT_STATE GetButtonState(int button)
		{
			const auto ret = m_button_states[button];
			if (ret)
				m_button_states[button] = INPUT_STATE::INPUT_STATE_REPEAT;
			return ret;
		}

	private:
		std::unordered_map<int, INPUT_STATE> m_key_states;
		std::array<INPUT_STATE, 8> m_button_states;
		glm::vec2 m_mouse_pos;
	};
} // namespace Mango