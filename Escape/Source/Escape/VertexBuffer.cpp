#include "EscapePCH.h"
#include "VertexBuffer.h"

#include <glad/glad.h>

namespace Escape {

	namespace Utils {

		static uint32 OpenGLVertexBufferUsage(VertexBufferUsage usage)
		{
			switch (usage)
			{
			case VertexBufferUsage::Static:  return GL_STATIC_DRAW;
			case VertexBufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
			}
			std::cerr << "Unknown vertex buffer usage!" << std::endl;
			return 0;
		}

	}

	VertexBuffer::VertexBuffer(const void* data, uint32 size, VertexBufferUsage usage)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, size, data, Utils::OpenGLVertexBufferUsage(usage));
	}

	VertexBuffer::VertexBuffer(uint32 size, VertexBufferUsage usage)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, size, nullptr, Utils::OpenGLVertexBufferUsage(usage));
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, uint32 size, uint32 offset)
	{
		glNamedBufferSubData(m_BufferID, offset, size, data);
	}

}