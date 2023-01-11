#pragma once

#include "Shader.h"

namespace esc {
	
	enum class PrimitiveTopology : uint8
	{
		Triangles = 0
	};

	struct GraphicsPipelineCreateInfo
	{
		Ref<Shader> Shader;
		PrimitiveTopology Topology = PrimitiveTopology::Triangles;
	};

	class GraphicsPipeline : public ReferenceCounted
	{
	public:
		GraphicsPipeline(const GraphicsPipelineCreateInfo& createInfo);
		virtual ~GraphicsPipeline();

		void Bind() const;
		void DrawIndexed(uint32 indexCount) const;
	private:
		GraphicsPipelineCreateInfo m_CreateInfo;
		uint32 m_VertexArrayID = 0;
	};

}