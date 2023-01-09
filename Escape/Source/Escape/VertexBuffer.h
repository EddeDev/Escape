#pragma once

namespace Escape {

	enum class VertexBufferUsage : uint8
	{
		Static = 0,
		Dynamic = 1
	};

	class VertexBuffer : public ReferenceCounted
	{
	public:
		VertexBuffer(const void* data, uint32 size, VertexBufferUsage usage = VertexBufferUsage::Static);
		VertexBuffer(uint32 size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
		virtual ~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uint32 size, uint32 offset = 0);
	private:
		uint32 m_BufferID = 0;
	};

}