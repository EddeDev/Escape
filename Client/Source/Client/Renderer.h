#pragma once

#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"

#include "Camera.h"

namespace esc {

	class Renderer : public ReferenceCounted
	{
	public:
		Renderer();
		virtual ~Renderer();

		void BeginScene(const Camera& camera);
		void EndScene();

		void FlushQuads();
		void RenderQuad(const glm::vec3& position, float angle = 0.0f, const glm::vec3& scale = glm::vec3(1.0f), const glm::vec4& color = glm::vec4(1.0f));

		void SetViewportSize(int32 width, int32 height);
	private:
		struct CameraDataUB
		{
			glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
			glm::mat4 InverseViewProjectionMatrix = glm::mat4(1.0f);
			glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
			glm::mat4 ViewMatrix = glm::mat4(1.0f);
		};

		CameraDataUB m_CameraData;
		Ref<UniformBuffer> m_CameraDataUniformBuffer;

		uint32 m_ViewportWidth = 0;
		uint32 m_ViewportHeight = 0;

		Ref<Shader> m_QuadShader;
		Ref<GraphicsPipeline> m_QuadPipeline;

		static const uint32 s_MaxQuads = 10000;
		static const uint32 s_MaxQuadVertices = s_MaxQuads * 4;
		static const uint32 s_MaxQuadIndices = s_MaxQuads * 6;

		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};

		Ref<VertexBuffer> m_QuadVertexBuffer;
		Ref<IndexBuffer> m_QuadIndexBuffer;
		QuadVertex* m_QuadVertexStorage = nullptr;
		QuadVertex* m_QuadVertexPointer = nullptr;
		glm::vec4 m_QuadVertexPositions[4];
		uint32 m_QuadIndexCount = 0;
	};

}