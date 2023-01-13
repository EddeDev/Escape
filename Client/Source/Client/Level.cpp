#include "ClientPCH.h"
#include "Level.h"

#include "EscapeGame.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

namespace esc {

	Level::Level(const LevelSpecification& specification)
	{
		Ref<Window> window = EscapeGame::Get().GetWindow();

		m_Renderer = Ref<Renderer>::Create();
		m_Renderer->SetViewportSize(window->GetWidth(), window->GetHeight());

		m_PhysicsWorld = new b2World({ 0.0f, -9.81f });

		EntityCreateInfo groundEntityCreateInfo;
		groundEntityCreateInfo.World = m_PhysicsWorld;
		groundEntityCreateInfo.IsDynamic = false;
		groundEntityCreateInfo.Position = { 0.0f, -10.0f };
		groundEntityCreateInfo.Scale = { 100.0f, 10.0f };
		groundEntityCreateInfo.Color = { 0.2f, 0.7f, 0.2f, 1.0f };
		groundEntityCreateInfo.DebugName = "Ground";
		m_GroundEntity = Ref<Entity>::Create(groundEntityCreateInfo);
		m_Entities.push_back(m_GroundEntity);

		EntityCreateInfo playerEntityCreateInfo;
		playerEntityCreateInfo.World = m_PhysicsWorld;
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
		obstacleEntityCreateInfo.World = m_PhysicsWorld;
		obstacleEntityCreateInfo.IsDynamic = true;
		obstacleEntityCreateInfo.Position = { 2.0f, 7.0f };
		obstacleEntityCreateInfo.Scale = { 2.0f, 2.0f };
		obstacleEntityCreateInfo.Color = { 0.4f, 0.6f, 0.7f, 1.0f };
		obstacleEntityCreateInfo.DebugName = "Obstacle";
		m_ObstacleEntity = Ref<Entity>::Create(obstacleEntityCreateInfo);
		m_Entities.push_back(m_ObstacleEntity);

		EntityCreateInfo obstacle2EntityCreateInfo;
		obstacle2EntityCreateInfo.World = m_PhysicsWorld;
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
				entityCreateInfo.World = m_PhysicsWorld;
				entityCreateInfo.IsDynamic = true;
				entityCreateInfo.Position = { 20.0f + (float)x * (1.0f + offset), 5.0f + (float)y * (1.0f + offset) };
				entityCreateInfo.Scale = { 1.0f, 1.0f };
				entityCreateInfo.Color = { 0.3f, 0.8f, 0.5f, 1.0f };
				m_Entities.push_back(Ref<Entity>::Create(entityCreateInfo));
			}
		}

		m_Camera = Camera(window->GetWidth(), window->GetHeight());
		m_Camera.SetZoomLevel(7.0f);
	}

	Level::~Level()
	{
		delete m_PhysicsWorld;
	}

	void Level::OnUpdate(float deltaTime)
	{
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
				m_PhysicsWorld->Step(m_FixedTimestep, m_VelocityIterations, m_PositionIterations);
		}

		m_Renderer->BeginScene(m_Camera);

		for (Ref<Entity> entity : m_Entities)
		{
			glm::vec3 position = { entity->GetPosition().x, entity->GetPosition().y, -0.2f };
			float angle = entity->GetAngle();
			glm::vec3 scale = { entity->GetScale().x, entity->GetScale().y, 1.0f };
			glm::vec4 color = entity->GetColor();

			m_Renderer->RenderQuad(position, angle, scale, color);
		}

		for (auto& [id, entity] : m_ClientPlayerEntities)
		{
			glm::vec3 position = { entity->GetPosition().x, entity->GetPosition().y, -0.2f };
			float angle = entity->GetAngle();
			glm::vec3 scale = { entity->GetScale().x, entity->GetScale().y, 1.0f };
			glm::vec4 color = entity->GetColor();

			m_Renderer->RenderQuad(position, angle, scale, color);
		}

		m_Renderer->EndScene();
	
		auto& clientData = EscapeGame::Get().GetClient()->GetClientData();
		for (const auto& [id, data] : clientData)
		{
			if (m_ClientPlayerEntities.find(id) == m_ClientPlayerEntities.end())
			{
				EntityCreateInfo playerEntityCreateInfo;
				playerEntityCreateInfo.World = m_PhysicsWorld;
				playerEntityCreateInfo.IsKinematic = true;
				playerEntityCreateInfo.FixedRotation = true;
				playerEntityCreateInfo.AllowSleep = false;
				playerEntityCreateInfo.Position = { data.LastTransformUpdate.PositionX, data.LastTransformUpdate.PositionY };
				playerEntityCreateInfo.Scale = { data.LastTransformUpdate.ScaleX, data.LastTransformUpdate.ScaleY };
				playerEntityCreateInfo.Color = { data.LastColorUpdate.ColorR, data.LastColorUpdate.ColorG, data.LastColorUpdate.ColorB, 1.0f };
				playerEntityCreateInfo.Friction = 0.0f;

				if (playerEntityCreateInfo.Scale.x == 0.0f || playerEntityCreateInfo.Scale.y == 0.0f)
					playerEntityCreateInfo.Scale = { 1.0f, 1.0f };

				m_ClientPlayerEntities[id] = Ref<Entity>::Create(playerEntityCreateInfo);
			}

			const auto& transform = data.LastTransformUpdate;
			const auto& color = data.LastColorUpdate;

			Ref<Entity> entity = m_ClientPlayerEntities.at(id);
			entity->SetPosition({ transform.PositionX, transform.PositionY });
			entity->SetAngle(transform.Angle);
			entity->SetScale({ transform.ScaleX, transform.ScaleY });

			// Temp
			entity->SetColor({ 0.7f, 0.7f, 0.9f, 1.0f });
		}
	}

	void Level::OnFixedUpdate()
	{
		float x = GetHorizontalAxis();

		float playerSpeed = 500.0f * m_FixedTimestep;
		if (EscapeGame::Get().GetKeyboard()->GetKey(KeyCode::LeftShift) || EscapeGame::Get().GetGamepad()->GetAxis(GamepadAxisCode::RightTrigger) > 0.0f)
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
			if (EscapeGame::Get().GetKeyboard()->GetKey(KeyCode::Space) || EscapeGame::Get().GetGamepad()->GetButton(GamepadButtonCode::A))
			{
				float jumpForce = 5.0f;
				m_PlayerEntity->SetLinearVelocity({ m_PlayerEntity->GetLinearVelocity().x, jumpForce });
			}
		}

		// Transform
		{
			TransformUpdatePacket update = {};
			update.PositionX = m_PlayerEntity->GetPosition().x;
			update.PositionY = m_PlayerEntity->GetPosition().y;
			update.Angle = m_PlayerEntity->GetAngle();
			update.ScaleX = m_PlayerEntity->GetScale().x;
			update.ScaleY = m_PlayerEntity->GetScale().y;

			if (update != m_LastTransformUpdate)
				EscapeGame::Get().GetClient()->SendPacket(PacketType::TransformUpdate, update);

			m_LastTransformUpdate = update;
		}
	}

	void Level::SetViewportSize(int32 width, int32 height)
	{
		m_Camera.SetViewportSize(width, height);
		m_Renderer->SetViewportSize(width, height);
	}

#define GAMEPAD_AXIS_OFFSET 0.1

	float Level::GetHorizontalAxis(bool right) const
	{
		float gamepadAxis = EscapeGame::Get().GetGamepad()->GetAxis(right ? GamepadAxisCode::RightX : GamepadAxisCode::LeftX);
		float keyboardAxis = EscapeGame::Get().GetKeyboard()->GetHorizontalAxis();

		float result = glm::clamp(gamepadAxis + keyboardAxis, -1.0f, 1.0f);
		if (result <= GAMEPAD_AXIS_OFFSET && result >= -GAMEPAD_AXIS_OFFSET)
			return 0.0f;

		return result;
	}

	float Level::GetVerticalAxis(bool right, bool gamepadInvert) const
	{
		float gamepadAxis = EscapeGame::Get().GetGamepad()->GetAxis(right ? GamepadAxisCode::RightY : GamepadAxisCode::LeftY);
		if (gamepadInvert)
			gamepadAxis = -gamepadAxis;

		float keyboardAxis = EscapeGame::Get().GetKeyboard()->GetVerticalAxis();

		float result = glm::clamp(gamepadAxis + keyboardAxis, -1.0f, 1.0f);
		if (result <= GAMEPAD_AXIS_OFFSET && result >= -GAMEPAD_AXIS_OFFSET)
			return 0.0f;

		return result;
	}

}