#include "ClientPCH.h"
#include "Renderer.h"

#include "ResourceManager.h"

#include <glad/glad.h>

namespace esc {

	Renderer::Renderer()
	{
		m_CameraDataUniformBuffer = Ref<UniformBuffer>::Create(static_cast<uint32>(sizeof(CameraDataUB)), 0);

		// Quads
		{
			m_QuadShader = ResourceManager::GetShader("Quad");

			GraphicsPipelineCreateInfo pipelineCreateInfo;
			pipelineCreateInfo.Shader = m_QuadShader;
			pipelineCreateInfo.Topology = PrimitiveTopology::Triangles;
			m_QuadPipeline = Ref<GraphicsPipeline>::Create(pipelineCreateInfo);

			m_QuadVertexBuffer = Ref<VertexBuffer>::Create(static_cast<uint32>(s_MaxQuadVertices * sizeof(QuadVertex)));
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
			
			m_QuadIndexBuffer = Ref<IndexBuffer>::Create(indices, static_cast<uint32>(s_MaxQuadIndices * sizeof(uint32)));
			delete[] indices;

			m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			m_QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
			m_QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
			m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

			m_QuadTextureCoords[0] = { 0.0f, 0.0f };
			m_QuadTextureCoords[1] = { 1.0f, 0.0f };
			m_QuadTextureCoords[2] = { 1.0f, 1.0f };
			m_QuadTextureCoords[3] = { 0.0f, 1.0f };
		}

		// White texture
		{
			constexpr uint32 whiteTextureData = 0xffffffff;

			TextureInfo whiteTextureInfo;
			whiteTextureInfo.Width = 1;
			whiteTextureInfo.Height = 1;
			whiteTextureInfo.Data = (uint8*)&whiteTextureData;

			m_TextureSlots[0] = Ref<Texture>::Create(whiteTextureInfo);
		}
	}

	Renderer::~Renderer()
	{
		delete[] m_QuadVertexStorage;
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		m_CameraData.ViewMatrix = camera.ViewMatrix;
		m_CameraData.ProjectionMatrix = camera.ProjectionMatrix;
		m_CameraData.ViewProjectionMatrix = camera.ViewProjectionMatrix;
		m_CameraData.InverseViewProjectionMatrix = camera.InverseViewProjectionMatrix;
		m_CameraDataUniformBuffer->SetData(&m_CameraData, sizeof(CameraDataUB));
		
		StartBatch();
	}

	void Renderer::EndScene()
	{
		FlushQuads();
	}

	void Renderer::StartBatch()
	{
		m_QuadVertexPointer = m_QuadVertexStorage;
		m_QuadIndexCount = 0;

		m_TextureSlotIndex = 1;
	}

	void Renderer::FlushQuads()
	{
		if (m_QuadIndexCount)
		{
			uint32 bufferSize = static_cast<uint32>((uint8*)m_QuadVertexPointer - (uint8*)m_QuadVertexStorage);
			m_QuadVertexBuffer->SetData(m_QuadVertexStorage, bufferSize);

			m_QuadVertexBuffer->Bind();
			m_QuadPipeline->Bind();
			m_QuadIndexBuffer->Bind();

			for (uint32 i = 0; i < m_TextureSlotIndex; i++)
				m_TextureSlots[i]->Bind(i);

			m_QuadShader->Bind();
			m_QuadPipeline->DrawIndexed(m_QuadIndexCount);
		}
	}

	void Renderer::RenderQuad(const glm::vec3& position, float angle, const glm::vec3& scale, const glm::vec4& color, Ref<Texture> texture, float tilingFactor)
	{
		if (m_QuadIndexCount >= s_MaxQuadIndices)
		{
			FlushQuads();
			StartBatch();
		}

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), scale);

		float textureIndex = 0.0f;
		if (texture)
		{
			for (uint32 i = 1; i < m_TextureSlotIndex; i++)
			{
				if (m_TextureSlots[i].Get() == texture.Get())
				{
					textureIndex = static_cast<float>(i);
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				if (m_TextureSlotIndex >= s_MaxTextureSlots)
					FlushQuads();

				textureIndex = (float)m_TextureSlotIndex;
				m_TextureSlots[m_TextureSlotIndex] = texture;
				m_TextureSlotIndex++;
			}
		}

		for (uint32 i = 0; i < 4; i++)
		{
			if (m_UseCustomColors)
				m_QuadVertexPointer->Color = m_CustomColors[i];
			else
				m_QuadVertexPointer->Color = color;

			m_QuadVertexPointer->Position = transform * m_QuadVertexPositions[i];
			m_QuadVertexPointer->TexCoord = m_QuadTextureCoords[i];
			m_QuadVertexPointer->TexIndex = textureIndex;
			m_QuadVertexPointer->TilingFactor = tilingFactor;
			m_QuadVertexPointer++;
		}

		m_UseCustomColors = false;
		m_QuadIndexCount += 6;
	}

	void Renderer::SetColors(const glm::vec4& c1, const glm::vec4& c2, const glm::vec4& c3, const glm::vec4& c4)
	{
		m_CustomColors[0] = c1;
		m_CustomColors[1] = c2;
		m_CustomColors[2] = c3;
		m_CustomColors[3] = c4;

		m_UseCustomColors = true;
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