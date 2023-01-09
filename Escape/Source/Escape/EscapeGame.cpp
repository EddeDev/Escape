#include "EscapePCH.h"
#include "EscapeGame.h"

#include <glm/glm.hpp>

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

		struct QuadVertex
		{
			glm::vec3 Position;
		};

		struct QuadIndex
		{
			uint32 V1;
			uint32 V2;
			uint32 V3;
		};

		QuadVertex quadVertices[4];
		quadVertices[0] = { { -0.5f, -0.5f, 0.0f } };
		quadVertices[1] = { {  0.5f, -0.5f, 0.0f } };
		quadVertices[2] = { {  0.5f,  0.5f, 0.0f } };
		quadVertices[3] = { { -0.5f,  0.5f, 0.0f } };

		QuadIndex quadIndices[2];
		quadIndices[0] = { 0, 1, 2 };
		quadIndices[1] = { 2, 3, 0 };

		GraphicsPipelineCreateInfo pipelineCreateInfo;
		pipelineCreateInfo.Shader = m_Shader;
		pipelineCreateInfo.Topology = PrimitiveTopology::Triangles;
		m_Pipeline = Ref<GraphicsPipeline>::Create(pipelineCreateInfo);

		m_VertexBuffer = Ref<VertexBuffer>::Create(quadVertices, sizeof(QuadVertex) * 4);
		m_IndexBuffer = Ref<IndexBuffer>::Create(quadIndices, sizeof(QuadIndex) * 2);

		while (m_Running)
		{
			m_Window->PollEvents();

			m_VertexBuffer->Bind();
			m_Pipeline->Bind();
			m_IndexBuffer->Bind();

			m_Shader->Bind();
			m_Pipeline->DrawIndexed(m_IndexBuffer->GetCount());

			m_Window->SwapBuffers();
		}
	}

}