#pragma once

#include "PrimitiveTypes.h"
#include "RefCounting.h"

#include "Async.h"
#include "TcpClient.h"

namespace esc {

	class TcpListener : public ReferenceCounted
	{
	public:
		TcpListener(uint16 port);
		virtual ~TcpListener();

		void Start();

		void BeginAcceptTcpClient(const AsyncCallbackFn& callback, const void* state);
		Ref<TcpClient> EndAcceptTcpClient(const AsyncResult& result);
	private:
		void* m_Server = nullptr;
	};

}