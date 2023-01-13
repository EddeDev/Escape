#include "ClientPCH.h"
#include "Client.h"

#include "Core/Packet.h"

namespace esc {

	Client::Client(const ClientCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		if (enet_initialize() != 0)
		{
			std::cerr << "Unable to initialize ENet!" << std::endl;
			__debugbreak();
			return;
		}

		m_Client = enet_host_create(nullptr, 1, 2, 0, 0);
		if (!m_Client)
		{
			std::cerr << "Failed to create ENet client!" << std::endl;
			__debugbreak();
			return;
		}

		ENetAddress address;
		enet_address_set_host(&address, createInfo.Address.c_str());
		address.port = createInfo.Port;

		m_Peer = enet_host_connect(m_Client, &address, 2, 0);
		if (!m_Peer)
		{
			std::cerr << "enet_host_connect failed" << std::endl;
			__debugbreak();
			return;
		}

		ENetEvent event = {};
		switch (enet_host_service(m_Client, &event, 5000))
		{
		case ENET_EVENT_TYPE_CONNECT:
		{
			std::cout << "Connection to " << createInfo.Address << ":" << createInfo.Port << " succeeded." << std::endl;
			break;
		}
		case ENET_EVENT_TYPE_DISCONNECT:
		{
			std::cout << "Connection to " << createInfo.Address << ":" << createInfo.Port << " failed." << std::endl;
			__debugbreak();
			break;
		}
		}

		char usernameData[80] = "2|";
		strcat(usernameData, createInfo.Username.c_str());
		ENetPacket* packet = enet_packet_create(usernameData, strlen(usernameData) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_Peer, 0, packet);
	}

	Client::~Client()
	{
		enet_deinitialize();
	}

	void Client::Update()
	{
		ENetEvent event = {};
		while (enet_host_service(m_Client, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
			{
				ParsePacketData(event.packet->data);
				enet_packet_destroy(event.packet);
				break;
			}
			}
		}
	}

	void Client::ParsePacketData(uint8* data)
	{
		PacketType type;
		int32 id;
		sscanf((const char*)data, "%d|%d", &type, &id);

		switch (type)
		{
		case PacketType::Username:
		{
			if (id != m_LocalID)
			{
				char username[80];
				memset(username, 0, 80);
				sscanf((const char*)data, "%*d|%*d|%[^|]", &username);
				m_ClientData[id].ID = id;
				m_ClientData[id].Username = username;
			}
			break;
		}
		case PacketType::ID:
		{
			m_LocalID = id;
			break;
		}
		case PacketType::TransformUpdate:
		{
			if (id != m_LocalID)
				m_ClientData[id].Transform = *(TransformUpdate*)((uint8*)data + 4);
			break;
		}
		}
	}

}