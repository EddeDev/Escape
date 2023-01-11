#include "ClientPCH.h"
#include "UniformBuffer.h"

#include <glad/glad.h>

namespace esc {

	UniformBuffer::UniformBuffer(uint32 size, uint32 binding)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, size, nullptr, GL_DYNAMIC_DRAW);

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_BufferID);
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void UniformBuffer::SetData(const void* data, uint32 size, uint32 offset)
	{
		glNamedBufferSubData(m_BufferID, offset, size, data);
	}

}