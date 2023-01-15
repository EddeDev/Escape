#pragma once

#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"
#include "Client.h"
#include "Level.h"

namespace esc {

	class EscapeGame
	{
	public:
		EscapeGame(const std::string& address, uint16 port, const std::string& username);
		~EscapeGame();

		void Run();
		void Close();

		Ref<Window> GetWindow() const { return m_Window; }
		Ref<Keyboard> GetKeyboard() const { return m_Keyboard; }
		Ref<Mouse> GetMouse() const { return m_Mouse; }
		Ref<Gamepad> GetGamepad() const { return m_Gamepad; }
		Ref<Client> GetClient() const { return m_Client; }

		static EscapeGame& Get() { return *s_Instance; }
	private:
		void OnWindowResize(int32 width, int32 height);
	private:
		inline static EscapeGame* s_Instance = nullptr;

		Ref<Window> m_Window;
		Ref<Keyboard> m_Keyboard;
		Ref<Mouse> m_Mouse;
		Ref<Gamepad> m_Gamepad;
		Ref<Client> m_Client;
		bool m_Running = true;

		Ref<Level> m_Level;
	};

}