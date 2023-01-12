#include "ServerPCH.h"
#include "Server.h"

#include "Client.h"

namespace esc {

	struct ServerData
	{
		uint32 MaxPlayers = 0;
		uint16 Port = 0;

		Ref<TcpListener> Listener;
		std::map<uint32, Ref<Client>> Clients;
	};

	static ServerData s_Data;

	void Server::Start(uint32 maxPlayers, uint16 port)
	{
		s_Data.MaxPlayers = maxPlayers;
		s_Data.Port = port;

		std::cout << "Starting server..." << std::endl;
		InitializeServerData();

		s_Data.Listener = Ref<TcpListener>::Create(port);
		s_Data.Listener->Start();
		s_Data.Listener->BeginAcceptTcpClient(ESCAPE_BIND_CALLBACK(TcpConnectCallback), nullptr);

		std::cout << "Server started on " << port << "." << std::endl;
	}

	void Server::TcpConnectCallback(const AsyncResult& result)
	{
		Ref<TcpClient> client = s_Data.Listener->EndAcceptTcpClient(result);
		s_Data.Listener->BeginAcceptTcpClient(ESCAPE_BIND_CALLBACK(TcpConnectCallback), nullptr);

		std::cout << "Incoming connection..." << std::endl; // TODO: print details

		for (uint32 i = 1; i <= s_Data.MaxPlayers; i++)
		{
			TCP* tcp = s_Data.Clients[i]->GetTCP();
			if (!tcp->GetSocket())
			{
				tcp->Connect(client);
				return;
			}
		}

		std::cerr << "Server is full!" << std::endl;
	}

	void Server::InitializeServerData()
	{
		for (uint32 i = 1; i <= s_Data.MaxPlayers; i++)
			s_Data.Clients[i] = Ref<Client>::Create(i);
	}

}