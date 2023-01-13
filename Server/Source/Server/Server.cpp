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
			m_Clients[i].ID = i;
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

			for (auto& [id, data] : m_Clients)
			{
				if (data.Peer)
				{
					char sendData[1024] = { '\0' };
					sprintf(sendData, "2|%d|%s", id, data.Username.c_str());

					ENetPacket* packet = enet_packet_create(sendData, strlen(sendData) + 1, ENET_PACKET_FLAG_RELIABLE);
					enet_host_broadcast(m_Server, 0, packet);
				}
			}

			m_Clients[i].Peer = peer;
			peer->data = &m_Clients[i].ID;

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
				std::cout << m_Clients[i].Username << " left the server!" << std::endl;

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

			char headerData[1024] = { '\0' };
			sprintf(headerData, "2|%d|%s", id, username);

			ENetPacket* packet = enet_packet_create(headerData, strlen(headerData) + 1, ENET_PACKET_FLAG_RELIABLE);
			enet_host_broadcast(m_Server, 0, packet);

			m_Clients[id].Username = username;

			auto& address = m_Clients[id].Peer->address;

			std::vector<char> ipAddressName(80);
			enet_address_get_host_ip(&address, ipAddressName.data(), ipAddressName.size());

			std::cout << username << " joined the server! (" << ipAddressName.data() << ":" << address.port << ")" << std::endl;

			break;
		}
		case PacketType::TransformUpdate:
		{
			struct TransformUpdate
			{
				float PositionX;
				float PositionY;
				float Angle;
				float ScaleX;
				float ScaleY;
			};

			TransformUpdate& update = *(TransformUpdate*)((uint8*)data + 2);

			char headerData[8] = { '\0' };
			sprintf(headerData, "4|%d|", id);

			size_t length = strlen(headerData);

			uint32 bufferSize = length + sizeof(TransformUpdate);
			uint8* buffer = new uint8[bufferSize];
			for (size_t i = 0; i < length; i++)
				buffer[i] = headerData[i];
			memcpy(&buffer[length], &update, sizeof(TransformUpdate));

			ENetPacket* packet = enet_packet_create(buffer, bufferSize, ENET_PACKET_FLAG_RELIABLE);
			enet_host_broadcast(m_Server, 0, packet);

			break;
		}
		}
	}

}