#include "ServerPCH.h"
#include "Server.h"

#include "Core/Packet.h"

#include "Time.h"

namespace esc {

	Server::Server(const ServerCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		std::cout << "Starting server..." << std::endl;

		// Initialize client map
		for (uint32 i = 1; i <= createInfo.MaxPlayers; i++)
		{
			m_Clients[i].Peer = nullptr;
			m_Clients[i].Username = "";
		}

		if (enet_initialize() != 0)
		{
			std::cerr << "Unable to initialize ENet!" << std::endl;
			__debugbreak();
			return;
		}

		ENetAddress address = {};
		address.host = ENET_HOST_ANY;
		address.port = createInfo.Port;

		m_Server = enet_host_create(&address, createInfo.MaxPlayers, 2, 0, 0);
		if (!m_Server)
		{
			std::cerr << "Failed to create ENet server host!" << std::endl;
			__debugbreak();
			return;
		}

		std::cout << "Server started on " << createInfo.Port << "." << std::endl;
	}

	Server::~Server()
	{
		enet_deinitialize();
	}

	void Server::Update()
	{
		ENetEvent event;
		while (enet_host_service(m_Server, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				OnClientConnect(event.peer);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				OnClientDisconnect(event.peer);
				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
			{
				if (event.peer->data)
				{
					int32 id = *(int32*)event.peer->data;
					ParseData(id, event.packet->data);
				}
				break;
			}
			}
		}
	}

	void Server::OnClientConnect(ENetPeer* peer)
	{
		for (uint32 i = 1; i <= m_CreateInfo.MaxPlayers; i++)
		{
			// Continue if the slot is occupied
			if (m_Clients[i].Peer)
				continue;

			m_Clients[i].Peer = peer;

			for (auto& [id, data] : m_Clients)
			{
				char sendData[1024] = { '\0' };
				sprintf(sendData, "2|%d|%s", id, data.Username.c_str());

				ENetPacket* packet = enet_packet_create(sendData, strlen(sendData) + 1, ENET_PACKET_FLAG_RELIABLE);
				enet_host_broadcast(m_Server, 0, packet);
			}

			peer->data = &i;

			char data[126] = { '\0' };
			sprintf(data, "3|%d", i);
			ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet);

			break;
		}
	}

	void Server::OnClientDisconnect(ENetPeer* peer)
	{
		for (uint32 i = 1; i <= m_CreateInfo.MaxPlayers; i++)
		{
			if (m_Clients[i].Peer == peer)
			{
				m_Clients[i].Peer = nullptr;
				m_Clients[i].Username = "";
				break;
			}
		}
	}

	void Server::PrintClientMap() const
	{
		std::cout << "Clients:" << std::endl;

		for (auto& [id, data] : m_Clients)
			std::cout << "Clients[" << id << "] = " << (data.Username.empty() ? "Null" : data.Username) << std::endl;
	}

	void Server::ParseData(int32 id, uint8* data)
	{
		PacketType type;
		sscanf((const char*)data, "%d|", &type);

		switch (type)
		{
		case PacketType::Username:
		{
			char username[80];
			sscanf((const char*)data, "2|%[^\n]", &username);

			char sendData[1024] = { '\0' };
			sprintf(sendData, "2|%d|%s", id, username);

			ENetPacket* packet = enet_packet_create(sendData, strlen(sendData) + 1, ENET_PACKET_FLAG_RELIABLE);
			enet_host_broadcast(m_Server, 0, packet);

			m_Clients[id].Username = username;

			auto& address = m_Clients[id].Peer->address;

			std::vector<char> ipAddressName(80);
			enet_address_get_host_ip(&address, ipAddressName.data(), ipAddressName.size());

			std::cout << username << " joined the server! (" << ipAddressName.data() << ":" << address.port << ")" << std::endl;

			break;
		}
		}
	}

}