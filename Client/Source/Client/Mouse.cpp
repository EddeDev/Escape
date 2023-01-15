#include "ClientPCH.h"
#include "Mouse.h"

namespace esc {

	Mouse::Mouse(Ref<Window>& window)
		: m_Window(window)
	{
		m_StateBitFields.resize((size_t)MouseButtonCode::ButtonLast);

		m_Window->AddMouseButtonPressCallback(ESCAPE_BIND_CALLBACK(OnMouseButtonPress, this));
		m_Window->AddMouseButtonReleaseCallback(ESCAPE_BIND_CALLBACK(OnMouseButtonRelease, this));

		m_MousePosition = m_Window->GetMousePosition();
	}

	void Mouse::Update()
	{
		glm::vec2 mousePosition = m_Window->GetMousePosition();
		// mousePosition.y = m_Window->GetHeight() - mousePosition.y;
		m_MousePositionDelta = m_MousePosition - mousePosition;
		mousePosition = glm::clamp(mousePosition, { 0.0f, 0.0f }, { m_Window->GetWidth(), m_Window->GetHeight() });
		m_MousePosition = mousePosition;

		// Reset bit fields
		for (size_t i = 0; i < m_StateBitFields.size(); i++)
			m_StateBitFields[i] = 0;
	}

	void Mouse::OnMouseButtonPress(int32 button)
	{
		m_StateBitFields[button] |= 0b00000001;
		m_StateBitFields[button] &= 0b11111101;
		m_StateBitFields[button] &= 0b11111011;
	}

	void Mouse::OnMouseButtonRelease(int32 button)
	{
		m_StateBitFields[button] &= 0b11111110;
		m_StateBitFields[button] |= 0b00000010;
		m_StateBitFields[button] &= 0b11111011;
	}

	bool Mouse::GetButton(MouseButtonCode button) const
	{
		return m_Window->IsMouseButtonDown((int32)button);
	}

	bool Mouse::GetButtonDown(MouseButtonCode button) const
	{
		return m_StateBitFields[(int32)button] & 0b00000001;
	}

	bool Mouse::GetButtonUp(MouseButtonCode button) const
	{
		return (m_StateBitFields[(int32)button] & 0b00000010) >> 1;
	}

	glm::vec2 Mouse::GetMouseOrthoPosition(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const
	{
		glm::vec4 result = { m_MousePosition, 0.0f, 1.0f };
		result.x = (result.x / (float)m_Window->GetWidth()) * 2.0f - 1.0f;
		result.y = (result.y / (float)m_Window->GetHeight()) * 2.0f - 1.0f;
		result.y = -result.y;
		result = glm::inverse(viewMatrix) * glm::inverse(projectionMatrix) * result;
		return result;
	}

}