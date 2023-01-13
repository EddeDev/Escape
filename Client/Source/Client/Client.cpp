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
		if (enet_host_service(m_Client, &event, 2000) &&
			event.type == ENET_EVENT_TYPE_CONNECT)
		{
			std::cout << "Connection to " << createInfo.Address << ":" << createInfo.Port << " succeeded." << std::endl;
			m_IsConnected = true;
		}
		else
		{
			std::cout << "Connection to " << createInfo.Address << ":" << createInfo.Port << " failed." << std::endl;
			m_IsConnected = false;
			return;
		}

		// Send username packet
		{
			UsernamePacket usernamePacket;
			strncpy_s(usernamePacket.Username, createInfo.Username.c_str(), createInfo.Username.size());
			SendPacket(PacketType::Username, usernamePacket);
		}
	}

	Client::~Client()
	{
		enet_deinitialize();
	}

	void Client::Update()
	{
		if (!m_IsConnected)
			return;

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
		PacketHeader header;
		memcpy(&header, data, sizeof(PacketHeader));

		if (header.Type == PacketType::ID)
		{
			m_LocalID = header.ID;
			return;
		}

		if (header.ID == m_LocalID)
			return;

		switch (header.Type)
		{
		case PacketType::Username:
		{
			UsernamePacket packet;
			memcpy(&packet, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(UsernamePacket));

			auto& clientData = m_ClientData[header.ID];
			clientData.ID = header.ID;
			clientData.Username = packet.Username;
			break;
		}
		case PacketType::TransformUpdate:
		{
			TransformUpdatePacket packet;
			memcpy(&packet, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(TransformUpdatePacket));

			auto& clientData = m_ClientData[header.ID];
			clientData.LastTransformPacket = packet;
			break;
		}
		}
	}

}