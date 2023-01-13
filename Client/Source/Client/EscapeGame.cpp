#include "ClientPCH.h"
#include "EscapeGame.h"

#include <glad/glad.h>

namespace esc {

	EscapeGame::EscapeGame(const std::string& address, uint16 port, const std::string& username)
	{
		s_Instance = this;

		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Width = 1280;
		windowCreateInfo.Height = 720;
		windowCreateInfo.Fullscreen = false;
		windowCreateInfo.Title = "Escape";

		m_Window = Ref<Window>::Create(windowCreateInfo);
		m_Window->AddCloseCallback(ESCAPE_BIND_CALLBACK(OnWindowClose, this));
		m_Window->AddResizeCallback(ESCAPE_BIND_CALLBACK(OnWindowResize, this));

		m_Keyboard = Ref<Keyboard>::Create(m_Window);
		m_Mouse = Ref<Mouse>::Create(m_Window);
		m_Gamepad = Ref<Gamepad>::Create(m_Window);

		ClientCreateInfo clientCreateInfo;
		clientCreateInfo.Address = address;
		clientCreateInfo.Port = port;
		clientCreateInfo.Username = username;

		m_Client = Ref<Client>::Create(clientCreateInfo);

		LevelSpecification levelSpec;
		m_Level = Ref<Level>::Create(levelSpec);
	}

	void EscapeGame::Run()
	{
		float lastTime = m_Window->GetTime();

		while (m_Running)
		{
			float time = m_Window->GetTime();
			float deltaTime = time - lastTime;
			lastTime = time;

			m_Window->PollEvents();
			if (m_Keyboard->GetKeyDown(KeyCode::Escape))
				m_Running = false;

			m_Client->Update();
			m_Level->OnUpdate(deltaTime);

			m_Keyboard->Update();
			m_Mouse->Update();

			m_Window->SwapBuffers();
		}
	}

	void EscapeGame::OnWindowClose()
	{
		m_Running = false;
	}

	void EscapeGame::OnWindowResize(int32 width, int32 height)
	{
		m_Level->SetViewportSize(width, height);
	}

}