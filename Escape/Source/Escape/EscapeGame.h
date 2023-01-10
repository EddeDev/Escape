#pragma once

#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"

#include "Shader.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"

#include "Renderer.h"

#include "Camera.h"
#include "Entity.h"

#include <box2d/b2_world.h>

namespace Escape {

	class EscapeGame
	{
	public:
		EscapeGame();
		~EscapeGame();

		void Run();

		b2World& GetPhysicsWorld() const { return *m_PhysicsWorld; }

		static EscapeGame& Get() { return *s_Instance; }
	private:
		void OnUpdate(float deltaTime);
		void OnFixedUpdate();

		void OnWindowClose();
		void OnWindowResize(int32 width, int32 height);
	private:
		inline static EscapeGame* s_Instance = nullptr;

		Ref<Window> m_Window;
		Ref<Keyboard> m_Keyboard;
		Ref<Mouse> m_Mouse;
		Ref<Renderer> m_Renderer;
		bool m_Running = true;

		b2World* m_PhysicsWorld = nullptr;
		float m_Accumulator = 0.0f;
		float m_FixedTimestep = 1.0f / 100.0f;
		uint32 m_NumSubSteps = 0;
		uint32 m_MaxSubSteps = 8;

		std::vector<Ref<Entity>> m_Entities;
		Ref<Entity> m_GroundEntity;
		Ref<Entity> m_ObstacleEntity;
		Ref<Entity> m_Obstacle2Entity;
		Ref<Entity> m_PlayerEntity;

		Camera m_Camera;
	};

}