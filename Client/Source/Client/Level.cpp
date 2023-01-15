#include "ClientPCH.h"
#include "Level.h"

#include "EscapeGame.h"
#include "ResourceManager.h"
#include "Random.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

#include <random>

namespace esc {

	Level::Level(const LevelSpecification& specification)
	{
		Ref<Window> window = EscapeGame::Get().GetWindow();

		m_Renderer = Ref<Renderer>::Create();
		m_Renderer->SetViewportSize(window->GetWidth(), window->GetHeight());

		m_PhysicsWorld = new b2World({ 0.0f, -9.81f });

		ChunkSpecification grassChunkSpec;
		grassChunkSpec.Mapping[TileType::Middle] = "Grass_Middle";
		grassChunkSpec.Mapping[TileType::TopSide] = "Grass_TopSide";
		grassChunkSpec.Mapping[TileType::BottomSide] = "Grass_BottomSide";
		grassChunkSpec.Mapping[TileType::LeftSide] = "Grass_LeftSide";
		grassChunkSpec.Mapping[TileType::RightSide] = "Grass_RightSide";
		grassChunkSpec.Mapping[TileType::TopLeftCorner] = "Grass_TopLeft";
		grassChunkSpec.Mapping[TileType::TopRightCorner] = "Grass_TopRight";
		grassChunkSpec.Mapping[TileType::BottomLeftCorner] = "Grass_BottomLeft";
		grassChunkSpec.Mapping[TileType::BottomRightCorner] = "Grass_BottomRight";

		// Chunk 1
		{
			EntityCreateInfo createInfo;
			createInfo.World = m_PhysicsWorld;
			createInfo.IsDynamic = false;
			createInfo.Position = { 0.0f, 0.0f };
			createInfo.Scale = { 20.0f, 5.0f };
			createInfo.DebugName = "Chunk 1";
			createInfo.ChunkSpec = &grassChunkSpec;
			m_GlobalEntities.push_back(Ref<Entity>::Create(createInfo));
		}

		// Chunk 2
		{
			EntityCreateInfo createInfo;
			createInfo.World = m_PhysicsWorld;
			createInfo.IsDynamic = false;
			createInfo.Position = { 25.0f, 0.0f };
			createInfo.Scale = { 20.0f, 5.0f };
			createInfo.DebugName = "Chunk 2";
			createInfo.ChunkSpec = &grassChunkSpec;
			m_GlobalEntities.push_back(Ref<Entity>::Create(createInfo));
		}

		// Chunk 3
		{
			EntityCreateInfo createInfo;
			createInfo.World = m_PhysicsWorld;
			createInfo.IsDynamic = false;
			createInfo.Position = { 45.0f, 5.0f };
			createInfo.Scale = { 10.0f, 5.0f };
			createInfo.DebugName = "Chunk 3";
			createInfo.ChunkSpec = &grassChunkSpec;
			m_GlobalEntities.push_back(Ref<Entity>::Create(createInfo));
		}

		// Player
		{
			EntityCreateInfo createInfo;
			createInfo.World = m_PhysicsWorld;
			createInfo.IsDynamic = true;
			createInfo.FixedRotation = true;
			createInfo.AllowSleep = false;
			createInfo.Position = { -2.0f, 10.0f };
			createInfo.Scale = { 1.0f, 2.0f };
			createInfo.Friction = 0.0f;
			createInfo.DebugName = "Player";
			
			m_PlayerSpawnPosition = createInfo.Position;

			std::random_device seeder;
			std::mt19937 engine(seeder());
			std::uniform_int_distribution<int> dist(1, 5);
			int32 random = dist(engine);

			createInfo.Texture = ResourceManager::GetTexture("TestChar" + std::to_string(random));

			m_PlayerEntity = Ref<Entity>::Create(createInfo);
		}

		// Bridge
		if (true)
		{
			EntityCreateInfo createInfo;
			createInfo.World = m_PhysicsWorld;
			createInfo.IsDynamic = true;
			createInfo.Position = { 5.0f, 5.0f };
			createInfo.Scale = { 10.0f, 0.5f };
			createInfo.Color = { 0.37f, 0.21f, 0.0f, 1.0f };
			m_GlobalEntities.push_back(Ref<Entity>::Create(createInfo));
		}

		m_CloudTexture = ResourceManager::GetTexture("CloudWithoutOutlines");

		if (true)
		{
			EntityCreateInfo createInfo;
			createInfo.World = m_PhysicsWorld;
			createInfo.IsDynamic = true;
			createInfo.Position = { 5.0f, 5.0f };
			createInfo.Scale = { 2.0f, 2.0f };
			createInfo.Texture = ResourceManager::GetTexture("Crate");
			m_GlobalEntities.push_back(Ref<Entity>::Create(createInfo));
		}
		
#if 0
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
				entityCreateInfo.Texture = ResourceManager::GetTexture("Crate");
				m_GlobalEntities.push_back(Ref<Entity>::Create(entityCreateInfo));
			}
		}
#endif

		m_Camera = Camera(window->GetWidth(), window->GetHeight());
		m_Camera.SetZoomLevel(7.0f);

		EscapeGame::Get().GetClient()->AddClientConnectCallback([this](auto clientID)
		{
			auto& clientData = EscapeGame::Get().GetClient()->GetClientData();
			if (clientData.find(clientID) == clientData.end())
			{
				std::cout << "Client is null!" << std::endl;
				__debugbreak();
			}
			m_SendPackets = true;
		});

		EscapeGame::Get().GetClient()->AddClientDisconnectCallback([this](auto clientID)
		{
			auto& clientData = EscapeGame::Get().GetClient()->GetClientData();
			if (clientData.find(clientID) == clientData.end())
			{
				std::cout << "Client is null!" << std::endl;
				__debugbreak();
			}
			if (clientData.at(clientID).Username.empty())
				__debugbreak();
			std::cout << clientData.at(clientID).Username << " left the game!" << std::endl;

			m_ClientPlayerEntities.erase(clientID);
		});

		EscapeGame::Get().GetClient()->AddEntityUpdateCallback([this](const auto& packet)
		{
			const EntityUpdatePacket& update = packet;
			
			Ref<Entity> entity = m_GlobalEntities[update.ID];
			entity->SetPosition({ update.PositionX, update.PositionY });
			entity->SetAngle(update.Angle);
			entity->SetScale({ update.ScaleX, update.ScaleY });
			entity->SetLinearVelocity({ update.VelocityX, update.VelocityY });
		});
	}

	Level::~Level()
	{
		delete m_PhysicsWorld;
	}

	void Level::OnUpdate(float deltaTime)
	{
		static float s_Time = 0.0f;
		s_Time += deltaTime;

		if (EscapeGame::Get().GetKeyboard()->GetKeyDown(KeyCode::P))
		{
			auto& clientData = EscapeGame::Get().GetClient()->GetClientData();
			std::cout << "Connected players:" << std::endl;
			for (auto& [id, data] : clientData)
				std::cout << "  " << data.Username << " (" << id << ")" << std::endl;
		}

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

		// Render sky
		{
			glm::vec3 scale = { m_Camera.GetZoomLevel(), m_Camera.GetZoomLevel(), 1.0 };
			float aspectRatio = m_Camera.GetAspectRatio();
			scale.x *= aspectRatio;
			scale *= 2.0f;

#if 1
			glm::vec4 bottom = { 0.61, 0.95, 0.93, 1.0f };
			glm::vec4 top = { 0.00, 0.35, 0.90, 1.0f };
#else
			glm::vec4 bottom = { 0.95f, 0.95f, 0.96f, 1.0f };
			glm::vec4 top = { 0.46f, 0.84f, 0.75f, 1.0f };
			bottom = glm::mix(bottom, top, 0.2f);
#endif

			m_Renderer->SetColors(bottom, bottom, top, top);
			m_Renderer->RenderQuad({ m_Camera.GetPosition(), -0.21f}, 0.0f, scale);
		}
		
		struct Cloud
		{
			float PosX = 5.0f;
			float PosY = 0.0f;
			float ScaleX = 5.0f;
			float ScaleY = 5.0f;
			float StartTime = 0.0f;
			float LocalTime = 0.0f;
			float AlphaMax = 0.7f;

			Cloud(float posX = 5.0f)
				: PosX(posX)
			{
				StartTime = s_Time;

				float scale = Random::Range(3.0f, 5.0f);
				ScaleX = scale * Random::Range(0.9f, 1.1f);
				ScaleY = scale * Random::Range(0.9f, 1.1f);

				AlphaMax = Random::Range(0.5f, 0.8f);
			}

			void Update(float deltaTime)
			{
				LocalTime += deltaTime;

				PosX -= deltaTime * 0.5f;
				PosY = 10.0f + glm::cos(LocalTime * 0.7f) * 0.5f;
			}

			void Render(Ref<Renderer> renderer, Ref<Texture> texture)
			{
				float alpha = glm::clamp((s_Time - StartTime) * 0.3f, 0.0f, AlphaMax);
				renderer->RenderQuad({ PosX, PosY, -0.205f }, 0.0f, { ScaleX, ScaleY, 1.0f }, { 1.0f, 1.0f, 1.0f, alpha }, texture, 1.0f);
			}
		};

		static std::vector<Cloud> clouds;

		static float t = 15.0f;
		t += deltaTime;
		if (t >= 15.0f)
		{
			clouds.push_back(Cloud(m_PlayerEntity->GetPosition().x + 10.0f));
			t = 0.0f;
		}

		for (size_t i = 0; i < clouds.size(); i++)
		{
			auto& cloud = clouds[i];
			if (cloud.LocalTime >= 100.0f)
			{
				clouds.erase(clouds.begin() + i);
				continue;
			}

			cloud.Update(deltaTime);
			cloud.Render(m_Renderer, m_CloudTexture);
		}

		for (Ref<Entity> entity : m_GlobalEntities)
		{
			glm::vec3 position = { entity->GetPosition().x, entity->GetPosition().y, -0.2f };
			float angle = entity->GetAngle();
			glm::vec3 scale = { entity->GetScale().x, entity->GetScale().y, 1.0f };
			glm::vec4 color = entity->GetColor();

			if (entity->IsChunk())
			{
				auto& textures = entity->GetChunkTextures();

				for (uint32 y = 0; y < (uint32)scale.y; y++)
				{
					for (uint32 x = 0; x < (uint32)scale.x; x++)
					{
						float deltaX = (scale.x * 0.5f - (float)x - 0.5f) * 1.00f;
						float deltaY = (scale.y * 0.5f - (float)y - 0.5f) * 1.00f;

						glm::vec3 currentPos = position + glm::vec3(deltaX, deltaY, 0.0f);
						glm::vec3 currentScale = { 1.0f, 1.0f, 1.0f };

						Ref<Texture> texture = nullptr;

						if (y == 0)
						{
							if (x == 0)
								texture = textures.at(TileType::TopRightCorner);
							else if (x == (uint32)scale.x - 1)
								texture = textures.at(TileType::TopLeftCorner);
							else
								texture = textures.at(TileType::TopSide);
						}

						if (y == (uint32)scale.y - 1)
						{
							if (x == 0)
								texture = textures.at(TileType::BottomRightCorner);
							else if (x == (uint32)scale.x - 1)
								texture = textures.at(TileType::BottomLeftCorner);
						}

						if (y != 0 && y != (uint32)scale.y - 1)
						{
							if (x == 0)
								texture = textures.at(TileType::RightSide);
							else if (x == (uint32)scale.x - 1)
								texture = textures.at(TileType::LeftSide);
						}

						if (x != 0 && x != (uint32)scale.x - 1)
						{
							if (y == (uint32)scale.y - 1)
								texture = textures.at(TileType::BottomSide);
						}

						if (!texture)
							texture = textures.at(TileType::Middle);
					
						m_Renderer->RenderQuad(currentPos, angle, currentScale, color, texture, 1.0f);
					}
				}
			}
			else
			{
				Ref<Texture> texture = entity->GetTexture();
				m_Renderer->RenderQuad(position, angle, scale, color, texture, 1.0f);
			}
		}

		if (m_PlayerEntity)
		{
			glm::vec3 position = { m_PlayerEntity->GetPosition().x, m_PlayerEntity->GetPosition().y, -0.2f };
			float angle = m_PlayerEntity->GetAngle();
			glm::vec3 scale = { m_PlayerEntity->GetScale().x, m_PlayerEntity->GetScale().y, 1.0f };
			glm::vec4 color = m_PlayerEntity->GetColor();

			Ref<Texture> texture = m_PlayerEntity->GetTexture();
			m_Renderer->RenderQuad(position, angle, scale, color, texture, 1.0f);
		}

		for (auto& [id, entity] : m_ClientPlayerEntities)
		{
			glm::vec3 position = { entity->GetPosition().x, entity->GetPosition().y, -0.2f };
			float angle = entity->GetAngle();
			glm::vec3 scale = { entity->GetScale().x, entity->GetScale().y, 1.0f };
			glm::vec4 color = entity->GetColor();
			Ref<Texture> texture = entity->GetTexture();

			m_Renderer->RenderQuad(position, angle, scale, color, texture);
		}

		m_Renderer->EndScene();
	
		const auto& clientData = EscapeGame::Get().GetClient()->GetClientData();
		for (auto& [id, data] : clientData)
		{
			if (m_ClientPlayerEntities.find(id) == m_ClientPlayerEntities.end())
			{
				EntityCreateInfo playerEntityCreateInfo;
				playerEntityCreateInfo.World = m_PhysicsWorld;
				playerEntityCreateInfo.IsDynamic = true;
				playerEntityCreateInfo.FixedRotation = true;
				playerEntityCreateInfo.AllowSleep = false;
				playerEntityCreateInfo.Position = { data.LatestTransformUpdate.PositionX, data.LatestTransformUpdate.PositionY };
				playerEntityCreateInfo.Scale = { data.LatestTransformUpdate.ScaleX, data.LatestTransformUpdate.ScaleY };
				playerEntityCreateInfo.Color = { data.LatestPlayerUpdate.ColorR, data.LatestPlayerUpdate.ColorG, data.LatestPlayerUpdate.ColorB, 1.0f };
				playerEntityCreateInfo.Friction = 0.0f;

				if (playerEntityCreateInfo.Scale.x == 0.0f || playerEntityCreateInfo.Scale.y == 0.0f)
					playerEntityCreateInfo.Scale = { 1.0f, 1.0f };

				m_ClientPlayerEntities[id] = Ref<Entity>::Create(playerEntityCreateInfo);
			}

			Ref<Entity> entity = m_ClientPlayerEntities.at(id);
			auto& materialData = data.LatestPlayerUpdate;
			entity->SetColor({ materialData.ColorR, materialData.ColorG, materialData.ColorB, 1.0f });

			Ref<Texture> texture = entity->GetTexture();
			if (texture)
			{
				std::string textureName = texture->GetInfo().GetName();
				if (strcmp(textureName.c_str(), materialData.TextureName) != 0)
				{
					Ref<Texture> newTexture = ResourceManager::GetTexture(materialData.TextureName);
					if (!newTexture)
					{
						std::cout << "Texture is null! (" << materialData.TextureName << ")" << std::endl;
						__debugbreak();
					}
					entity->SetTexture(newTexture);
				}
			}
			else
			{
				Ref<Texture> newTexture = ResourceManager::GetTexture(materialData.TextureName);
				entity->SetTexture(newTexture);
			}
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

		if (m_PlayerEntity->GetPosition().y < -35.0f)
		{
			m_PlayerEntity->SetPosition(m_PlayerSpawnPosition);
			m_Camera.SetPosition(glm::lerp(m_Camera.GetPosition(), m_PlayerEntity->GetPosition(), cameraMoveSpeed * 10.0f));
		}

		const float jumpForce = 5.0f;

		bool isGrounded = m_PlayerEntity->IsGrounded();
		if (isGrounded)
		{
			if (EscapeGame::Get().GetKeyboard()->GetKey(KeyCode::Space) || EscapeGame::Get().GetGamepad()->GetButton(GamepadButtonCode::A))
				m_PlayerEntity->SetLinearVelocity({ m_PlayerEntity->GetLinearVelocity().x, jumpForce });
		}

		const auto& clientData = EscapeGame::Get().GetClient()->GetClientData();
		for (auto& [id, data] : clientData)
		{
			if (m_ClientPlayerEntities.find(id) == m_ClientPlayerEntities.end())
				continue;

			Ref<Entity> entity = m_ClientPlayerEntities[id];
			auto& input = data.LatestInputUpdate;
			entity->SetLinearVelocity({ input.HorizontalAxis * input.Speed, entity->GetLinearVelocity().y });

			if (entity->IsGrounded())
			{
				if (input.Jump)
					entity->SetLinearVelocity({ entity->GetLinearVelocity().x, jumpForce });
			}

			glm::vec2 deltaVelocity = { input.VelocityX - entity->GetLinearVelocity().x, input.VelocityY - entity->GetLinearVelocity().y };
			entity->SetLinearVelocity(entity->GetLinearVelocity() + deltaVelocity);
		}

		// Input
		{
			InputPacket packet = {};
			packet.HorizontalAxis = x;
			packet.VerticalAxis = y;
			packet.VelocityX = m_PlayerEntity->GetLinearVelocity().x;
			packet.VelocityY = m_PlayerEntity->GetLinearVelocity().y;
			packet.Speed = playerSpeed;
			packet.Jump = EscapeGame::Get().GetKeyboard()->GetKey(KeyCode::Space) || EscapeGame::Get().GetGamepad()->GetButton(GamepadButtonCode::A);

			if (packet != m_LatestInputPacket || m_SendPackets)
			{
				// std::cout << "Sending Input packet!" << std::endl;
				EscapeGame::Get().GetClient()->SendPacket(PacketType::Input, packet);
				m_SentPackets++;
			}

			m_LatestInputPacket = packet;
		}

		// Transform
		{
			TransformUpdatePacket update = {};
			update.PositionX = m_PlayerEntity->GetPosition().x;
			update.PositionY = m_PlayerEntity->GetPosition().y;
			update.Angle = m_PlayerEntity->GetAngle();
			update.ScaleX = m_PlayerEntity->GetScale().x;
			update.ScaleY = m_PlayerEntity->GetScale().y;

			if (update != m_LatestTransformUpdate || m_SendPackets)
			{
				// std::cout << "Sending TransformUpdate packet!" << std::endl;
				EscapeGame::Get().GetClient()->SendPacket(PacketType::TransformUpdate, update);
				m_SentPackets++;
			}

			m_LatestTransformUpdate = update;
		}

		// Player Update (Material data)
		{
			PlayerUpdatePacket update = {};
			std::string textureName = m_PlayerEntity->GetTexture()->GetInfo().GetName();
			strncpy_s(update.TextureName, textureName.c_str(), textureName.size());
			update.ColorR = m_PlayerEntity->GetColor().r;
			update.ColorG = m_PlayerEntity->GetColor().g;
			update.ColorB = m_PlayerEntity->GetColor().b;

			if (update != m_LatestPlayerUpdate || m_SendPackets)
			{
				// std::cout << "Sending PlayerUpdate packet!" << std::endl;
				// std::cout << "  Texture Data: " << std::endl;
				// std::cout << "    Name: " << textureName << " / " << update.TextureName << std::endl;
				// std::cout << "    Filepath: " << m_PlayerEntity->GetTexture()->GetInfo().Filepath << std::endl;
				// std::cout << "    Has Data: " << (m_PlayerEntity->GetTexture()->GetInfo().Data ? "true" : "false") << std::endl;
				EscapeGame::Get().GetClient()->SendPacket(PacketType::PlayerUpdate, update);
				m_SentPackets++;
			}

			m_LatestPlayerUpdate = update;
		}

		// Physics Data
		{
			PhysicsDataPacket data = {};

			for (auto& entity : m_GlobalEntities)
				data.NumPhysicsEntities++;

			for (auto& [id, entity] : m_ClientPlayerEntities)
				data.NumPhysicsEntities++;

			if (data != m_LatestPhysicsData || m_SendPackets)
			{
				// std::cout << "Sending PhysicsData packet!" << std::endl;
				EscapeGame::Get().GetClient()->SendPacket(PacketType::PhysicsData, data);
				m_SentPackets++;
			}

			m_LatestPhysicsData = data;
		}

		// Entity Update
		{
#define PRINT_MOVING_ENTITIES 0

#if PRINT_MOVING_ENTITIES
			std::cout << "Moving entities: ";
			uint32 entityPrintIndex = 0;
#endif
			for (size_t i = 0; i < m_GlobalEntities.size(); i++)
			{
				Ref<Entity> entity = m_GlobalEntities[i];

				auto velocity = entity->GetLinearVelocity();
				const float fct = 0.1f;
				// TODO: optimize (contacts?)
				if (velocity.x > fct || velocity.x < -fct || velocity.y > fct || velocity.y < -fct || m_SendPackets)
				{
#if PRINT_MOVING_ENTITIES
					if (entityPrintIndex > 0)
						std::cout << ", ";
					std::cout << entity->GetDebugName();
					entityPrintIndex++;
#endif

					EntityUpdatePacket update = {};
					update.ID = i;
					update.PositionX = entity->GetPosition().x;
					update.PositionY = entity->GetPosition().y;
					update.Angle = entity->GetAngle();
					update.ScaleX = entity->GetScale().x;
					update.ScaleY = entity->GetScale().y;
					update.VelocityX = velocity.x;
					update.VelocityY = velocity.y;

					if (update != m_LatestEntityUpdateMap[i] || m_SendPackets)
					{
						// std::cout << "Sending EntityUpdate packet! (" << update.ID << ")" << std::endl;
						// EscapeGame::Get().GetClient()->SendPacket(PacketType::EntityUpdate, update);
						m_SentPackets++;
					}

					m_LatestEntityUpdateMap[i] = update;
				}
			}

#if PRINT_MOVING_ENTITIES
			std::cout << std::endl;
#endif
		}

		if (m_SendPackets)
		{
			// TODO: Print something
			m_SendPackets = false;
		}
		
		// std::cout << "Sent packets: " << m_SendPackets << std::endl;

		m_PacketsPerFrame = m_SentPackets;
		m_SentPackets = 0;

		for (auto& [id, data] : clientData)
		{
			if (m_ClientPlayerEntities.find(id) != m_ClientPlayerEntities.end())
			{
				Ref<Entity> entity = m_ClientPlayerEntities.at(id);

				auto& transform = data.LatestTransformUpdate;
				glm::vec2 position = { transform.PositionX, transform.PositionY };
				float angle = transform.Angle;
				glm::vec2 scale = { transform.ScaleX, transform.ScaleY };

#define LERP 0
#if LERP
				entity->SetPosition(glm::lerp(entity->GetPosition(), position, 10.0f * m_FixedTimestep));
				entity->SetAngle(glm::lerp(entity->GetAngle(), angle, 10.0f * m_FixedTimestep));
				entity->SetScale(glm::lerp(entity->GetScale(), scale, 10.0f * m_FixedTimestep));
#else
				entity->SetPosition(position);
				entity->SetAngle(angle);
				entity->SetScale(scale);
#endif
			}
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