#pragma once

namespace Escape {

	class UniformBuffer : public ReferenceCounted
	{
	public:
		UniformBuffer(uint32 size, uint32 binding);
		virtual ~UniformBuffer();

		void SetData(const void* data, uint32 size, uint32 offset = 0);
	private:
		uint32 m_BufferID = 0;
	};

}