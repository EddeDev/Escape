#include "ClientPCH.h"
#include "Mouse.h"

namespace esc {

	Mouse::Mouse(Ref<Window> window)
		: m_Window(window)
	{
		m_StateBitFields.resize(MouseButtonCode::ButtonLast);

		m_Window->AddMouseButtonPressCallback(ESCAPE_BIND_CALLBACK(OnMouseButtonPress, this));
		m_Window->AddMouseButtonReleaseCallback(ESCAPE_BIND_CALLBACK(OnMouseButtonRelease, this));
	}

	void Mouse::Update()
	{
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
		return m_Window->IsMouseButtonDown(button);
	}

	bool Mouse::GetButtonDown(MouseButtonCode button) const
	{
		return m_StateBitFields[button] & 0b00000001;
	}

	bool Mouse::GetButtonUp(MouseButtonCode button) const
	{
		return (m_StateBitFields[button] & 0b00000010) >> 1;
	}

}