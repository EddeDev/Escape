#pragma once

#include "Window.h"

#include <glm/glm.hpp>

namespace esc {

	enum class MouseButtonCode : int32
	{
		Button1 = 0,
		Button2 = 1,
		Button3 = 2,
		Button4 = 3,
		Button5 = 4,
		Button6 = 5,
		Button7 = 6,
		Button8 = 7,

		ButtonLast = Button8,
		ButtonLeft = Button1,
		ButtonRight = Button2,
		ButtonMiddle = Button3
	};

	class Mouse : public ReferenceCounted
	{
	public:
		Mouse(Ref<Window>& window);
		virtual ~Mouse() {}

		void Update();

		bool GetButton(MouseButtonCode button) const;
		bool GetButtonDown(MouseButtonCode button) const;
		bool GetButtonUp(MouseButtonCode button) const;

		glm::vec2 GetMousePosition() const { return m_MousePosition; }
		glm::vec2 GetMouseOrthoPosition(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const;
	private:
		void OnMouseButtonPress(int32 button);
		void OnMouseButtonRelease(int32 button);
	private:
		Ref<Window>& m_Window;
		std::vector<uint8> m_StateBitFields;

		glm::vec2 m_MousePosition = glm::vec2(0.0f);
		glm::vec2 m_MousePositionDelta = glm::vec2(0.0f);
	};

}