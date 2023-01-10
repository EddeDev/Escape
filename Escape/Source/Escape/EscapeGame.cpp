#include "EscapePCH.h"
#include "EscapeGame.h"

#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Escape {

	EscapeGame::EscapeGame()
	{
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Width = 1280;
		windowCreateInfo.Height = 720;
		windowCreateInfo.Title = "Escape";

		m_Window = Ref<Window>::Create(windowCreateInfo);
		m_Window->AddCloseCallback(ESCAPE_BIND_CALLBACK(OnWindowClose, this));
		m_Window->AddResizeCallback(ESCAPE_BIND_CALLBACK(OnWindowResize, this));
	}

	EscapeGame::~EscapeGame()
	{

	}

	void EscapeGame::Run()
	{
		m_Renderer = Ref<Renderer>::Create();
		m_Renderer->SetViewportSize(m_Window->GetWidth(), m_Window->GetHeight());

		m_Camera = Camera(m_Window->GetWidth(), m_Window->GetHeight());
		m_Camera.SetZoomLevel(7.0f);

		while (m_Running)
		{
			m_Window->PollEvents();

			m_Renderer->BeginScene(m_Camera);
			m_Renderer->RenderQuad({ 0.0f, -5.0f, 0.0f }, { 10.0f, 1.0f, 1.0f }, { 0.2f, 0.7f, 0.2f, 1.0f });

			// Player A
			m_Renderer->RenderQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 2.0f, 1.0f }, { 0.8f, 0.2f, 0.2f, 1.0f });

			// Player B
			m_Renderer->RenderQuad({ 2.0f, 0.0f, 0.0f }, { 1.0f, 2.0f, 1.0f }, { 0.2f, 0.2f, 0.8f, 1.0f });

			m_Renderer->EndScene();

			m_Window->SwapBuffers();
		}
	}

	void EscapeGame::OnWindowClose()
	{
		m_Running = false;
	}

	void EscapeGame::OnWindowResize(int32 width, int32 height)
	{
		m_Camera.SetViewportSize(width, height);
		m_Renderer->SetViewportSize(width, height);
	}

}