#include "EscapePCH.h"
#include "EscapeGame.h"

#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Escape {

	EscapeGame::EscapeGame()
	{
		s_Instance = this;

		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Width = 1280;
		windowCreateInfo.Height = 720;
		windowCreateInfo.Title = "Escape";

		m_Window = Ref<Window>::Create(windowCreateInfo);
		m_Window->AddCloseCallback(ESCAPE_BIND_CALLBACK(OnWindowClose, this));
		m_Window->AddResizeCallback(ESCAPE_BIND_CALLBACK(OnWindowResize, this));

		m_Keyboard = Ref<Keyboard>::Create(m_Window);
		m_Mouse = Ref<Mouse>::Create(m_Window);
	}

	EscapeGame::~EscapeGame()
	{
	}

	void EscapeGame::Run()
	{
		m_Renderer = Ref<Renderer>::Create();
		m_Renderer->SetViewportSize(m_Window->GetWidth(), m_Window->GetHeight());
		
		m_PhysicsWorld = new b2World({ 0.0f, -9.81f });

		EntityCreateInfo groundEntityCreateInfo;
		groundEntityCreateInfo.IsDynamic = false;
		groundEntityCreateInfo.Position = { 0.0f, -5.0f };
		groundEntityCreateInfo.Scale = { 100.0f, 1.0f };
		groundEntityCreateInfo.Color = { 0.2f, 0.7f, 0.2f, 1.0f };
		groundEntityCreateInfo.DebugName = "Ground";
		m_GroundEntity = Ref<Entity>::Create(groundEntityCreateInfo);
		m_Entities.push_back(m_GroundEntity);

		EntityCreateInfo playerEntityCreateInfo;
		playerEntityCreateInfo.IsDynamic = true;
		playerEntityCreateInfo.FixedRotation = true;
		playerEntityCreateInfo.AllowSleep = false;
		playerEntityCreateInfo.Position = { -2.0f, 0.0f };
		playerEntityCreateInfo.Scale = { 1.0f, 2.0f };
		playerEntityCreateInfo.Color = { 0.8f, 0.2f, 0.2f, 1.0f };
		playerEntityCreateInfo.Friction = 0.0f;
		playerEntityCreateInfo.DebugName = "Player";
		m_PlayerEntity = Ref<Entity>::Create(playerEntityCreateInfo);
		m_Entities.push_back(m_PlayerEntity);

		EntityCreateInfo obstacleEntityCreateInfo;
		obstacleEntityCreateInfo.IsDynamic = true;
		obstacleEntityCreateInfo.Position = { 2.0f, 7.0f };
		obstacleEntityCreateInfo.Scale = { 2.0f, 2.0f };
		obstacleEntityCreateInfo.Color = { 0.4f, 0.6f, 0.7f, 1.0f };
		obstacleEntityCreateInfo.DebugName = "Obstacle";
		m_ObstacleEntity = Ref<Entity>::Create(obstacleEntityCreateInfo);
		m_Entities.push_back(m_ObstacleEntity);

		EntityCreateInfo obstacle2EntityCreateInfo;
		obstacle2EntityCreateInfo.IsDynamic = false;
		obstacle2EntityCreateInfo.Position = { 5.0f, -4.0f };
		obstacle2EntityCreateInfo.Scale = { 1.0f, 1.0f };
		obstacle2EntityCreateInfo.Color = { 0.4f, 0.6f, 0.7f, 1.0f };
		obstacle2EntityCreateInfo.DebugName = "Obstacle";
		m_Obstacle2Entity = Ref<Entity>::Create(obstacle2EntityCreateInfo);
		m_Entities.push_back(m_Obstacle2Entity);

		m_Camera = Camera(m_Window->GetWidth(), m_Window->GetHeight());
		m_Camera.SetZoomLevel(7.0f);

		int32 velocityIterations = 8;
		int32 positionIterations = 3;

		float lastTime = m_Window->GetTime();

		while (m_Running)
		{
			float time = m_Window->GetTime();
			float deltaTime = time - lastTime;
			lastTime = time;

			m_Window->PollEvents();

			if (m_Keyboard->GetKeyDown(KeyCode::Escape))
				m_Running = false;

			Update(deltaTime);
			m_PhysicsWorld->Step(deltaTime, velocityIterations, positionIterations);

			m_Keyboard->Update();
			m_Mouse->Update();

			m_Renderer->BeginScene(m_Camera);

			for (auto& entity : m_Entities)
			{
				glm::vec3 position = { entity->GetPosition().x, entity->GetPosition().y, 0.0f };
				float angle = entity->GetAngle();
				glm::vec3 scale = { entity->GetScale().x, entity->GetScale().y, 1.0f };
				glm::vec4 color = entity->GetColor();

				m_Renderer->RenderQuad(position, angle, scale, color);
			}

			m_Renderer->EndScene();

			m_Window->SwapBuffers();
		}
	}

	void EscapeGame::Update(float deltaTime)
	{
		float x = m_Keyboard->GetHorizontalAxis();
		float y = m_Keyboard->GetVerticalAxis();

		float playerSpeed = 1000.0f * deltaTime;
		m_PlayerEntity->SetLinearVelocity({ x * playerSpeed, m_PlayerEntity->GetLinearVelocity().y });

#if 0
		bool isGrounded = m_PlayerEntity->IsTouching(m_GroundEntity);
#else
		bool isGrounded = m_PlayerEntity->IsGrounded();
#endif
		if (isGrounded)
		{
			if (m_Keyboard->GetKey(KeyCode::Space))
			{
				float jumpForce = 5.0f;
				m_PlayerEntity->SetLinearVelocity({ 0.0f, jumpForce });
			}
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