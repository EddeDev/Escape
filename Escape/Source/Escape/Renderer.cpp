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

			m_QuadVertexBuffer = Ref<VertexBuffer>::Create(quadVertices, sizeof(QuadVertex) * 4);
			m_QuadIndexBuffer = Ref<IndexBuffer>::Create(quadIndices, sizeof(QuadIndex) * 2);
		}
	}

	Renderer::~Renderer()
	{

	}

	void Renderer::BeginScene(const Camera& camera)
	{
		m_CameraData.ViewMatrix = camera.ViewMatrix;
		m_CameraData.ProjectionMatrix = camera.ProjectionMatrix;
		m_CameraData.ViewProjectionMatrix = camera.ViewProjectionMatrix;
		m_CameraData.InverseViewProjectionMatrix = camera.InverseViewProjectionMatrix;

		m_CameraDataUniformBuffer->SetData(&m_CameraData, sizeof(CameraDataUB));
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::RenderQuad(const glm::vec3& position)
	{
		m_QuadVertexBuffer->Bind();
		m_QuadPipeline->Bind();
		m_QuadIndexBuffer->Bind();

		m_QuadShader->Bind();
		m_QuadShader->SetUniform("u_Constants.Transform", glm::translate(glm::mat4(1.0f), position));

		m_QuadPipeline->DrawIndexed(m_QuadIndexBuffer->GetCount());
	}

	void Renderer::SetViewportSize(int32 width, int32 height)
	{
		glViewport(0, 0, width, height);
	}

}