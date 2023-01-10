#include "EscapePCH.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace Escape {

	Renderer::Renderer()
	{
		m_CameraDataUniformBuffer = Ref<UniformBuffer>::Create(sizeof(CameraDataUB), 0);

		// Quads
		{
			m_QuadShader = Ref<Shader>::Create("Resources/Shaders/VertexShader.glsl", "Resources/Shaders/FragmentShader.glsl");

			GraphicsPipelineCreateInfo pipelineCreateInfo;
			pipelineCreateInfo.Shader = m_QuadShader;
			pipelineCreateInfo.Topology = PrimitiveTopology::Triangles;
			m_QuadPipeline = Ref<GraphicsPipeline>::Create(pipelineCreateInfo);

			m_QuadVertexBuffer = Ref<VertexBuffer>::Create(s_MaxQuadVertices * sizeof(QuadVertex));
			m_QuadVertexStorage = new QuadVertex[s_MaxQuadVertices];
			memset(m_QuadVertexStorage, 0, s_MaxQuadVertices * sizeof(QuadVertex));

			uint32* indices = new uint32[s_MaxQuadIndices];
			uint32 offset = 0;
			for (uint32 i = 0; i < s_MaxQuadIndices; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}
			
			m_QuadIndexBuffer = Ref<IndexBuffer>::Create(indices, s_MaxQuadIndices * sizeof(uint32));
			delete[] indices;

			m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			m_QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
			m_QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
			m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		}
	}

	Renderer::~Renderer()
	{
		delete[] m_QuadVertexStorage;
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		m_QuadVertexPointer = m_QuadVertexStorage;
		m_QuadIndexCount = 0;

		m_CameraData.ViewMatrix = camera.ViewMatrix;
		m_CameraData.ProjectionMatrix = camera.ProjectionMatrix;
		m_CameraData.ViewProjectionMatrix = camera.ViewProjectionMatrix;
		m_CameraData.InverseViewProjectionMatrix = camera.InverseViewProjectionMatrix;
		m_CameraDataUniformBuffer->SetData(&m_CameraData, sizeof(CameraDataUB));
	}

	void Renderer::EndScene()
	{
		FlushQuads();
	}

	void Renderer::FlushQuads()
	{
		if (m_QuadIndexCount)
		{
			size_t bufferSize = static_cast<size_t>((uint8*)m_QuadVertexPointer - (uint8*)m_QuadVertexStorage);
			m_QuadVertexBuffer->SetData(m_QuadVertexStorage, bufferSize);

			m_QuadVertexBuffer->Bind();
			m_QuadPipeline->Bind();
			m_QuadIndexBuffer->Bind();

			m_QuadShader->Bind();
			m_QuadPipeline->DrawIndexed(m_QuadIndexBuffer->GetCount());
		}

		m_QuadVertexPointer = m_QuadVertexStorage;
		m_QuadIndexCount = 0;
	}

	void Renderer::RenderQuad(const glm::vec3& position, float angle, const glm::vec3& scale, const glm::vec4& color)
	{
		if (m_QuadIndexCount >= s_MaxQuadIndices)
			FlushQuads();

		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), scale);

		for (uint32 i = 0; i < 4; i++)
		{
			m_QuadVertexPointer->Position = transform * m_QuadVertexPositions[i];
			m_QuadVertexPointer->Color = color;
			m_QuadVertexPointer++;
		}

		m_QuadIndexCount += 6;
	}

	void Renderer::SetViewportSize(int32 width, int32 height)
	{
		if (m_ViewportWidth != width || m_ViewportHeight != height)
		{
			glViewport(0, 0, width, height);

			m_ViewportWidth = width;
			m_ViewportHeight = height;
		}
	}

}