#include "ClientPCH.h"
#include "Gamepad.h"

#include <glfw/glfw3.h>

namespace esc {

	Gamepad::Gamepad(Ref<Window> window)
		: m_Window(window)
	{
		s_Instance = this;

		for (uint32 i = 0; i < GLFW_JOYSTICK_LAST; i++)
		{
			int32 isPresent = glfwJoystickPresent(i);
			if (isPresent)
			{
				const char* joystickName = glfwGetJoystickName(i);
				const char* gamepadName = glfwGetGamepadName(i);
				std::cout << "Using gamepad: " << joystickName << " / " << gamepadName << std::endl;

				m_SelectedJoystickID = i;
				break;
			}
		}

		glfwSetJoystickCallback([](auto jid, auto event)
		{
			switch (event)
			{
			case GLFW_CONNECTED:
			{
				s_Instance->OnConnect(jid);
				break;
			}
			case GLFW_DISCONNECTED:
			{
				s_Instance->OnDisconnect(jid);
				break;
			}
			}
		});
	}

	void Gamepad::OnConnect(uint32 id)
	{
		m_SelectedJoystickID = id;
	}

	void Gamepad::OnDisconnect(uint32 id)
	{
		if (m_SelectedJoystickID == id)
		{
			m_SelectedJoystickID = -1;

			for (uint32 i = 0; i < GLFW_JOYSTICK_LAST; i++)
			{
				int32 isPresent = glfwJoystickPresent(i);
				if (isPresent)
				{
					m_SelectedJoystickID = i;
					break;
				}
			}
		}
	}

	void Gamepad::Update()
	{
	}

	bool Gamepad::GetButton(GamepadButtonCode button) const
	{
		if (m_SelectedJoystickID == -1)
			return 0.0f;

		if (glfwJoystickIsGamepad(m_SelectedJoystickID))
		{
			GLFWgamepadstate state;
			if (glfwGetGamepadState(m_SelectedJoystickID, &state))
				return state.buttons[static_cast<int32>(button)] == GLFW_PRESS;
		}

		return 0.0f;
	}

	bool Gamepad::GetButtonUp(GamepadButtonCode button) const
	{
		if (m_SelectedJoystickID == -1)
			return 0.0f;

		if (glfwJoystickIsGamepad(m_SelectedJoystickID))
		{
			GLFWgamepadstate state;
			if (glfwGetGamepadState(m_SelectedJoystickID, &state))
				return state.buttons[static_cast<int32>(button)] == GLFW_RELEASE;
		}

		return 0.0f;
	}

	float Gamepad::GetAxis(GamepadAxisCode axis) const
	{
		if (m_SelectedJoystickID == -1)
			return 0.0f;

		if (glfwJoystickIsGamepad(m_SelectedJoystickID))
		{
			GLFWgamepadstate state;
			if (glfwGetGamepadState(m_SelectedJoystickID, &state))
				return state.axes[static_cast<int32>(axis)];
		}

		return 0.0f;
	}

}