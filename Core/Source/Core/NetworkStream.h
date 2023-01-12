#pragma once

#include "PrimitiveTypes.h"
#include "RefCounting.h"

#include "Async.h"

namespace esc {

	struct NetworkStreamCreateInfo
	{

	};

	class NetworkStream : public ReferenceCounted
	{
	public:
		NetworkStream(const NetworkStreamCreateInfo& createInfo)
		{

		}

		virtual ~NetworkStream()
		{

		}

		void BeginRead(uint8* buffer, uint32 offset, uint32 size, const AsyncCallbackFn& callback, const void* state)
		{

		}

		int32 EndRead(const AsyncResult& result)
		{
			return 0;
		}
	};

}