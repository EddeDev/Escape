#include "EscapePCH.h"
#include "GraphicsPipeline.h"

#include <glad/glad.h>

namespace Escape {

	namespace Utils {

		static uint32 OpenGLShaderDataType(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float:  return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Int:    return GL_INT;
			case ShaderDataType::Int2:   return GL_INT;
			case ShaderDataType::Int3:   return GL_INT;
			case ShaderDataType::Int4:   return GL_INT;
			}
			std::cerr << "Unknown shader data type!" << std::endl;
			return 0;
		}

		static uint32 OpenGLPrimitiveTopology(PrimitiveTopology topology)
		{
			switch (topology)
			{
			case PrimitiveTopology::Triangles: return GL_TRIANGLES;
			}
			std::cerr << "Unknown primitive topology!" << std::endl;
			return 0;
		}

	}

	GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		glCreateVertexArrays(1, &m_VertexArrayID);
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void GraphicsPipeline::Bind() const
	{
		glBindVertexArray(m_VertexArrayID);

		auto& inputLayout = m_CreateInfo.Shader->GetInputLayout();
		for (auto& attribute : inputLayout)
		{
			glEnableVertexAttribArray(attribute.Index);

			uint32 type = Utils::OpenGLShaderDataType(attribute.Type);
			if (type == GL_INT)
				glVertexAttribIPointer(attribute.Index, attribute.ComponentCount, type, inputLayout.Stride, (const void*)(uintptr_t)attribute.Offset);
			else
				glVertexAttribPointer(attribute.Index, attribute.ComponentCount, type, GL_FALSE, inputLayout.Stride, (const void*)(uintptr_t)attribute.Offset);
		}
	}

	void GraphicsPipeline::DrawIndexed(uint32 indexCount) const
	{
		glDrawElements(Utils::OpenGLPrimitiveTopology(m_CreateInfo.Topology), indexCount, GL_UNSIGNED_INT, nullptr);
	}

}