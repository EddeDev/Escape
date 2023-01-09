#pragma once

#include "Window.h"

#include "Shader.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"

#include "Renderer.h"

#include "Camera.h"

namespace Escape {

	class EscapeGame
	{
	public:
		EscapeGame();
		~EscapeGame();

		void Run();
	private:
		void OnWindowClose();
		void OnWindowResize(int32 width, int32 height);
	private:
		Ref<Window> m_Window;
		Ref<Renderer> m_Renderer;
		bool m_Running = true;

		Camera m_Camera;
	};

}