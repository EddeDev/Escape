#include "ClientPCH.h"
#include "Keyboard.h"

namespace esc {

	Keyboard::Keyboard(Ref<Window>& window)
		: m_Window(window)
	{
		m_StateBitFields.resize(KeyCode::Last);

		window->AddKeyPressCallback(ESCAPE_BIND_CALLBACK(OnKeyPress, this));
		window->AddKeyReleaseCallback(ESCAPE_BIND_CALLBACK(OnKeyRelease, this));
		window->AddKeyRepeatCallback(ESCAPE_BIND_CALLBACK(OnKeyRepeat, this));
	}

	void Keyboard::Update()
	{
		// Reset bit fields
		for (size_t i = 0; i < m_StateBitFields.size(); i++)
			m_StateBitFields[i] = 0;
	}

	void Keyboard::OnKeyPress(int32 key)
	{
		m_StateBitFields[key] |= 0b00000001;
		m_StateBitFields[key] &= 0b11111101;
		m_StateBitFields[key] &= 0b11111011;
	}

	void Keyboard::OnKeyRelease(int32 key)
	{
		m_StateBitFields[key] &= 0b11111110;
		m_StateBitFields[key] |= 0b00000010;
		m_StateBitFields[key] &= 0b11111011;
	}

	void Keyboard::OnKeyRepeat(int32 key)
	{
		m_StateBitFields[key] &= 0b11111110;
		m_StateBitFields[key] &= 0b11111101;
		m_StateBitFields[key] |= 0b00000100;
	}

	bool Keyboard::GetKey(KeyCode key) const
	{
		return m_Window->IsKeyDown(key);
	}

	bool Keyboard::GetKeyDown(KeyCode key) const
	{
		return m_StateBitFields[key] & 0b00000001;
	}

	bool Keyboard::GetKeyUp(KeyCode key) const
	{
		return (m_StateBitFields[key] & 0b00000010) >> 1;
	}

	float Keyboard::GetHorizontalAxis() const
	{
		bool a = GetKey(KeyCode::A);
		bool d = GetKey(KeyCode::D);
		return a && d ? 0.0f : a ? -1.0f : d ? 1.0f : 0.0f;
	}

	float Keyboard::GetVerticalAxis() const
	{
		bool s = GetKey(KeyCode::S);
		bool w = GetKey(KeyCode::W);
		return s && w ? 0.0f : s ? -1.0f : w ? 1.0f : 0.0f;
	}

}