#pragma once

#include "PrimitiveTypes.h"
#include "RefCounting.h"

#include "Async.h"
#include "TcpClient.h"

#include <string>

namespace esc {

	struct TcpListenerCreateInfo
	{
		std::string Address = "127.0.0.1";
		uint16 Port = 0;
	};

	class TcpListener : public ReferenceCounted
	{
	public:
		TcpListener(const TcpListenerCreateInfo& createInfo)
		{

		}

		virtual ~TcpListener()
		{

		}

		void Start()
		{

		}

		void BeginAcceptTcpClient(const AsyncCallbackFn& callback, const void* state)
		{

		}

		Ref<TcpClient> EndAcceptTcpClient(const AsyncResult& result)
		{
			return nullptr;
		}
	};

}