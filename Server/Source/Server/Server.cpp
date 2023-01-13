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
					PacketHeader header;
					header.ID = id;
					header.Type = PacketType::Username;

					UsernamePacket usernamePacket;
					strncpy_s(usernamePacket.Username, data.Username.c_str(), data.Username.size());

					uint32 bufferSize = sizeof(PacketHeader) + sizeof(UsernamePacket);
					uint8* buffer = new uint8[bufferSize];
					memcpy(buffer, &header, sizeof(PacketHeader));
					memcpy(static_cast<uint8*>(buffer + sizeof(PacketHeader)), &usernamePacket, sizeof(UsernamePacket));

					ENetPacket* packet = enet_packet_create(buffer, bufferSize, ENET_PACKET_FLAG_RELIABLE);
					enet_host_broadcast(m_Server, 0, packet);

					delete[] buffer;
				}
			}

			peer->data = &m_Clients[i].ID;
			m_Clients[i].Peer = peer;

			// Send ID packet
			{
				PacketHeader header;
				header.ID = i;
				header.Type = PacketType::ID;

				IDPacket idPacket;
				idPacket.ID = i;

				uint32 bufferSize = sizeof(PacketHeader) + sizeof(IDPacket);
				uint8* buffer = new uint8[bufferSize];
				memcpy(buffer, &header, sizeof(PacketHeader));
				memcpy(static_cast<uint8*>(buffer + sizeof(PacketHeader)), &idPacket, sizeof(IDPacket));

				ENetPacket* packet = enet_packet_create(buffer, bufferSize, ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer, 0, packet);

				delete[] buffer;
			}

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
		PacketHeader header;
		memcpy(&header, data, sizeof(PacketHeader));

		if (header.Type == PacketType::ID)
		{
			__debugbreak();
			return;
		}

		switch (header.Type)
		{
		case PacketType::Username:
		{
			UsernamePacket usernamePacket;
			memcpy(&usernamePacket, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(UsernamePacket));

			uint32 bufferSize = sizeof(PacketHeader) + sizeof(UsernamePacket);
			uint8* buffer = new uint8[bufferSize];
			memcpy(buffer, &header, sizeof(PacketHeader));
			memcpy(static_cast<uint8*>(buffer + sizeof(PacketHeader)), &usernamePacket, sizeof(UsernamePacket));

			ENetPacket* packet = enet_packet_create(buffer, bufferSize, ENET_PACKET_FLAG_RELIABLE);
			enet_host_broadcast(m_Server, 0, packet);

			delete[] buffer;

			m_Clients[id].Username = usernamePacket.Username;

			{
				auto& address = m_Clients[id].Peer->address;
				std::vector<char> ipAddressName(80);
				enet_address_get_host_ip(&address, ipAddressName.data(), ipAddressName.size());
				std::cout << usernamePacket.Username << " joined the server! (" << ipAddressName.data() << ":" << address.port << ")" << std::endl;
			}
			
			break;
		}
		case PacketType::TransformUpdate:
		{
			TransformUpdatePacket transformPacket;
			memcpy(&transformPacket, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(TransformUpdatePacket));

			uint32 bufferSize = sizeof(PacketHeader) + sizeof(TransformUpdatePacket);
			uint8* buffer = new uint8[bufferSize];
			memcpy(buffer, &header, sizeof(PacketHeader));
			memcpy(static_cast<uint8*>(buffer + sizeof(PacketHeader)), &transformPacket, sizeof(TransformUpdatePacket));

			ENetPacket* packet = enet_packet_create(buffer, bufferSize, ENET_PACKET_FLAG_RELIABLE);
			enet_host_broadcast(m_Server, 0, packet);

			delete[] buffer;
			break;
		}
		}
	}

}