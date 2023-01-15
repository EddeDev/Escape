#pragma once

#include "Core/Packet.h"

#include "Entity.h"
#include "Renderer.h"
#include "Camera.h"
#include "Chunk.h"

#include <box2d/b2_world.h>
#include <box2d/b2_draw.h>

namespace esc {

	struct LevelSpecification
	{
	};

	class Level : public ReferenceCounted
	{
	public:
		Level(const LevelSpecification& specification);
		virtual ~Level();

		void OnUpdate(float deltaTime);
		void SetViewportSize(int32 width, int32 height);

		b2World& GetPhysicsWorld() const { return *m_PhysicsWorld; }
	private:
		void OnFixedUpdate();

		float GetHorizontalAxis(bool right = false) const;
		float GetVerticalAxis(bool right = false, bool gamepadInvert = false) const;
	private:
		Ref<Renderer> m_Renderer;
	
		// Physics
		b2World* m_PhysicsWorld = nullptr;
		float m_Accumulator = 0.0f;
		float m_FixedTimestep = static_cast<float>(ESCAPE_FIXED_TIMESTEP);
		uint32 m_NumSubSteps = 0;
		uint32 m_MaxSubSteps = 8;
		int32 m_VelocityIterations = 8;
		int32 m_PositionIterations = 3;

		std::vector<Ref<Entity>> m_GlobalEntities;
		std::unordered_map<int32, Ref<Entity>> m_ClientPlayerEntities;
		Ref<Entity> m_PlayerEntity;
		glm::vec2 m_PlayerSpawnPosition;

		Ref<Texture> m_CloudTexture;
		
		Camera m_Camera;

		InputPacket m_LatestInputPacket;
		TransformUpdatePacket m_LatestTransformUpdate;
		PlayerUpdatePacket m_LatestPlayerUpdate;
		PhysicsDataPacket m_LatestPhysicsData;
		std::unordered_map<int32, EntityUpdatePacket> m_LatestEntityUpdateMap;
		bool m_SendPackets = false;
		uint32 m_SentPackets = 0;
		uint32 m_PacketsPerFrame = 0;
	};

}