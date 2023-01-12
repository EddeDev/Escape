#pragma once

#include "PrimitiveTypes.h"
#include "RefCounting.h"

#include "Async.h"
#include "NetworkStream.h"

#include <string>

namespace esc {

	class TcpClient : public ReferenceCounted
	{
	public:
		TcpClient()
		{

		}

		virtual ~TcpClient()
		{

		}

		void BeginConnect(const std::string& address, uint16 port, const AsyncCallbackFn& requestCallback, const void* state)
		{

		}

		void EndConnect(const AsyncResult& result)
		{

		}

		void SetReceiveBufferSize(uint32 size)
		{

		}

		void SetSendBufferSize(uint32 size)
		{

		}

		Ref<NetworkStream> GetStream() const
		{
			return nullptr;
		}

		bool IsConnected() const
		{
			return false;
		}
	};

}