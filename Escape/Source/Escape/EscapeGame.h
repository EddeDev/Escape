#pragma once

#include "Window.h"

#include "Shader.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"

namespace Escape {

	class EscapeGame
	{
	public:
		EscapeGame();
		~EscapeGame();

		void Run();
	private:
		Ref<Window> m_Window;
		Ref<Shader> m_Shader;
		Ref<GraphicsPipeline> m_Pipeline;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<UniformBuffer> m_UniformBuffer;
		bool m_Running = true;
	};

}