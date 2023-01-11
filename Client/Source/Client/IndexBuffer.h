#pragma once

namespace esc {

	class IndexBuffer : public ReferenceCounted
	{
	public:
		IndexBuffer(const void* data, uint32 size);
		virtual ~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32 GetCount() const { return m_Size / sizeof(uint32); }
	private:
		uint32 m_Size = 0;
		uint32 m_BufferID = 0;
	};

}