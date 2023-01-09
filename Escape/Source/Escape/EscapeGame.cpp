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
		m_Window->AddCloseCallback([this]()
		{
			m_Running = false;
		});
	}

	EscapeGame::~EscapeGame()
	{

	}

	void EscapeGame::Run()
	{
		m_Shader = Ref<Shader>::Create("Resources/Shaders/VertexShader.glsl", "Resources/Shaders/FragmentShader.glsl");

		while (m_Running)
		{
			m_Window->PollEvents();
			m_Window->SwapBuffers();
		}
	}

}