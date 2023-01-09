#pragma once

#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"

#include "Camera.h"

namespace Escape {

	class Renderer : public ReferenceCounted
	{
	public:
		Renderer();
		virtual ~Renderer();

		void BeginScene(const Camera& camera);
		void EndScene();

		void RenderQuad(const glm::vec3& position);

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

		Ref<Shader> m_QuadShader;
		Ref<GraphicsPipeline> m_QuadPipeline;
		Ref<VertexBuffer> m_QuadVertexBuffer;
		Ref<IndexBuffer> m_QuadIndexBuffer;
	};

}