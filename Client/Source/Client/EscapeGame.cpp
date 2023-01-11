#include "ClientPCH.h"
#include "EscapeGame.h"

#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

namespace esc {

	EscapeGame::EscapeGame(const std::string& address, uint16 port)
	{
		s_Instance = this;

		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Width = 1280;
		windowCreateInfo.Height = 720;
		windowCreateInfo.Fullscreen = true;
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
		clientCreateInfo.NoDelay = true;
		clientCreateInfo.Blocking = false;
		clientCreateInfo.KeepAlive = false;
		clientCreateInfo.ReuseAddress = false;
		clientCreateInfo.TrafficClass = 24;
		clientCreateInfo.Timeout = 100;
		m_Client = Ref<Client>::Create(clientCreateInfo);
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
		groundEntityCreateInfo.Position = { 0.0f, -10.0f };
		groundEntityCreateInfo.Scale = { 100.0f, 10.0f };
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
		obstacle2EntityCreateInfo.Position = { 5.0f, -4.5f };
		obstacle2EntityCreateInfo.Scale = { 1.0f, 1.0f };
		obstacle2EntityCreateInfo.Color = { 0.4f, 0.6f, 0.7f, 1.0f };
		obstacle2EntityCreateInfo.DebugName = "Obstacle";
		m_Obstacle2Entity = Ref<Entity>::Create(obstacle2EntityCreateInfo);
		m_Entities.push_back(m_Obstacle2Entity);

		for (uint32 x = 0; x < 8; x++)
		{
			for (uint32 y = 0; y < 8; y++)
			{
				float offset = 0.05f;

				EntityCreateInfo entityCreateInfo;
				entityCreateInfo.IsDynamic = true;
				entityCreateInfo.Position = { 20.0f + (float)x * (1.0f + offset), 5.0f + (float)y * (1.0f + offset) };
				entityCreateInfo.Scale = { 1.0f, 1.0f };
				entityCreateInfo.Color = { 0.3f, 0.8f, 0.5f, 1.0f };
				m_Entities.push_back(Ref<Entity>::Create(entityCreateInfo));
			}
		}

		m_Camera = Camera(m_Window->GetWidth(), m_Window->GetHeight());
		m_Camera.SetZoomLevel(7.0f);

		int32 velocityIterations = 8;
		int32 positionIterations = 3;

		float lastTime = m_Window->GetTime();
		float lastFrameTime = m_Window->GetTime();
		uint32 frames = 0;

		while (m_Running)
		{
			float time = m_Window->GetTime();
			float deltaTime = time - lastTime;
			lastTime = time;

			frames++;
			while (time >= lastFrameTime + 1.0f)
			{
				std::cout << frames << " fps" << std::endl;
				lastFrameTime += 1.0f;
				frames = 0;
			}

			m_Window->PollEvents();

			if (m_Keyboard->GetKeyDown(KeyCode::Escape))
				m_Running = false;

			OnUpdate(deltaTime);

			if (m_Accumulator > m_FixedTimestep)
				m_Accumulator = 0.0f;

			m_Accumulator += deltaTime;
			if (m_Accumulator < m_FixedTimestep)
			{
				m_NumSubSteps = 0;
			}
			else
			{
				m_NumSubSteps = glm::min((uint32)(m_Accumulator / m_FixedTimestep), m_MaxSubSteps);
				m_Accumulator -= (float)m_NumSubSteps * m_FixedTimestep;
			}

			if (m_NumSubSteps > 0)
			{
				OnFixedUpdate();

				for (uint32 i = 0; i < m_NumSubSteps; i++)
					m_PhysicsWorld->Step(m_FixedTimestep, velocityIterations, positionIterations);
			}

			m_Keyboard->Update();
			m_Mouse->Update();

			m_Renderer->BeginScene(m_Camera);

			for (Ref<Entity> entity : m_Entities)
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

	void EscapeGame::OnUpdate(float deltaTime)
	{
		
	}

	void EscapeGame::OnFixedUpdate()
	{
		float x = GetHorizontalAxis();

		float playerSpeed = 500.0f * m_FixedTimestep;
		if (m_Keyboard->GetKey(KeyCode::LeftShift) || m_Gamepad->GetAxis(GamepadAxisCode::RightTrigger) > 0.0f)
			playerSpeed *= 2.0f;

		float y = GetVerticalAxis(true, true);
		m_Camera.SetZoomLevel(m_Camera.GetZoomLevel() + (-y * 2.0f * m_FixedTimestep));

		m_PlayerEntity->SetLinearVelocity({ x * playerSpeed, m_PlayerEntity->GetLinearVelocity().y });

		float cameraMoveSpeed = 2.0f * m_FixedTimestep;
		m_Camera.SetPosition(glm::lerp(m_Camera.GetPosition(), m_PlayerEntity->GetPosition(), cameraMoveSpeed));

#if 0
		bool isGrounded = m_PlayerEntity->IsTouching(m_GroundEntity);
#else
		bool isGrounded = m_PlayerEntity->IsGrounded();
#endif
		if (isGrounded)
		{
			if (m_Keyboard->GetKey(KeyCode::Space) || m_Gamepad->GetButton(GamepadButtonCode::A))
			{
				float jumpForce = 5.0f;
				m_PlayerEntity->SetLinearVelocity({ 0.0f, jumpForce });
			}
		}
	}

#define GAMEPAD_AXIS_OFFSET 0.1

	float EscapeGame::GetHorizontalAxis(bool right) const
	{
		float gamepadAxis = m_Gamepad->GetAxis(right ? GamepadAxisCode::RightX : GamepadAxisCode::LeftX);
		float keyboardAxis = m_Keyboard->GetHorizontalAxis();

		float result = glm::clamp(gamepadAxis + keyboardAxis, -1.0f, 1.0f);
		if (result <= GAMEPAD_AXIS_OFFSET && result >= -GAMEPAD_AXIS_OFFSET)
			return 0.0f;

		return result;
	}

	float EscapeGame::GetVerticalAxis(bool right, bool gamepadInvert) const
	{
		float gamepadAxis = m_Gamepad->GetAxis(right ? GamepadAxisCode::RightY : GamepadAxisCode::LeftY);
		if (gamepadInvert)
			gamepadAxis = -gamepadAxis;

		float keyboardAxis = m_Keyboard->GetVerticalAxis();

		float result = glm::clamp(gamepadAxis + keyboardAxis, -1.0f, 1.0f);
		if (result <= GAMEPAD_AXIS_OFFSET && result >= -GAMEPAD_AXIS_OFFSET)
			return 0.0f;

		return result;
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