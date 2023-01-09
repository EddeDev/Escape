#include "EscapePCH.h"
#include "EscapeGame.h"

namespace Escape {

	EscapeGame::EscapeGame()
	{
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Width = 1280;
		windowCreateInfo.Height = 720;
		windowCreateInfo.Title = "Escape";

		m_Window = Ref<Window>::Create(windowCreateInfo);
	}

	EscapeGame::~EscapeGame()
	{

	}

	void EscapeGame::Run()
	{
		while (m_Running)
		{
			m_Window->PollEvents();
			m_Window->SwapBuffers();
		}
	}

}