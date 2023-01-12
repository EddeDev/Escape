#pragma once

#include "Core/TcpListener.h"

namespace esc {

	class Server
	{
	public:
		static void Start(uint32 maxPlayers, uint16 port);
	private:
		static void TcpConnectCallback(const AsyncResult& result);
		static void InitializeServerData();
	};

}