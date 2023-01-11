#pragma once

#include "Window.h"

namespace esc {

	enum class GamepadButtonCode : uint8
	{
		A = 0,
		B = 1,
		X = 2,
		Y = 3,
		LeftBumper = 4,
		RightBumper = 5,
		Back = 6,
		Start = 7,
		Guide = 8,
		LeftThumb = 9,
		RightThumb = 10,
		DPadUp = 11,
		DPadRight = 12,
		DPadDown = 13,
		DPadLeft = 14,
		Last = DPadLeft,

		Cross = A,
		Circle = B,
		Square = X,
		Triangle = Y
	};

	enum class GamepadAxisCode : uint8
	{
		LeftX = 0,
		LeftY = 1,
		RightX = 2,
		RightY = 3,
		LeftTrigger = 4,
		RightTrigger = 5,
		Last = RightTrigger
	};

	class Gamepad : public ReferenceCounted
	{
	public:
		Gamepad(Ref<Window> window);
		virtual ~Gamepad() {}

		void Update();

		bool GetButton(GamepadButtonCode button)const;
		bool GetButtonUp(GamepadButtonCode button)const;
		float GetAxis(GamepadAxisCode axis) const;

		bool IsPresent() const { return m_SelectedJoystickID != -1; }
	private:
		void OnConnect(uint32 id);
		void OnDisconnect(uint32 id);
	private:
		inline static Gamepad* s_Instance = nullptr;

		Ref<Window> m_Window;
	
		int32 m_SelectedJoystickID = -1;
	};

}