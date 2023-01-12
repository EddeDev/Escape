#include "TcpListener.h"

#include <enet/enet.h>

namespace esc {

	TcpListener::TcpListener(uint16 port)
	{
		if (enet_initialize() != 0)
		{
			std::cerr << "Unable to initialize ENet!" << std::endl;
			__debugbreak();
			return;
		}

		ENetAddress address = {};
		address.host = ENET_HOST_ANY;
		address.port = port;

		m_Server = enet_host_create(&address, 32, 2, 0, 0);
		if (!m_Server)
		{
			std::cerr << "Failed to create ENet server host!" << std::endl;
			__debugbreak();
			return;
		}
	}

	TcpListener::~TcpListener()
	{
		enet_host_destroy((ENetHost*)m_Server);
	}

	void TcpListener::Start()
	{

	}

	void TcpListener::BeginAcceptTcpClient(const AsyncCallbackFn& callback, const void* state)
	{

	}

	Ref<TcpClient> TcpListener::EndAcceptTcpClient(const AsyncResult& result)
	{
		return nullptr;
	}

}