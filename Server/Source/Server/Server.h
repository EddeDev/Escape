#pragma once

#include <enet/enet.h>

namespace esc {

	struct ServerCreateInfo
	{
		uint32 MaxPlayers = 0;
		uint16 Port = 0;
	};

	class Server
	{
	public:
		Server(const ServerCreateInfo& createInfo);
		~Server();

		void Update();
	private:
		void OnClientConnect(ENetPeer* peer);
		void OnClientDisconnect(ENetPeer* peer);
	
		void PrintClientMap() const;

		void ParseData(int32 id, uint8* data);
	private:
		ServerCreateInfo m_CreateInfo;
		ENetHost* m_Server = nullptr;

		struct ClientData
		{
			ENetPeer* Peer = nullptr;
			std::string Username = "";
		};
		std::map<uint32, ClientData> m_Clients;
	};

}