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
		if (enet_host_service(m_Client, &event, 500) &&
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

		// Send connect packet
		{
			ConnectPacket connectPacket;
			strncpy_s(connectPacket.Username, createInfo.Username.c_str(), createInfo.Username.size());
			SendPacket(PacketType::Connect, connectPacket);
		}
	}

	Client::~Client()
	{
		enet_peer_disconnect(m_Peer, 0);
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

		switch (header.Type)
		{
		case PacketType::ID:
		{
			IDPacket packet;
			memcpy(&packet, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(IDPacket));
			m_LocalID = packet.ID;
			break;
		}
		case PacketType::Connect:
		{
			if (header.ID != m_LocalID)
			{
				ConnectPacket packet;
				memcpy(&packet, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(ConnectPacket));

				auto& clientData = m_ClientData[header.ID];
				clientData.ID = header.ID;
				clientData.Username = packet.Username;

				for (auto& callback : m_ClientConnectCallbacks)
					callback(clientData.ID);
			}
			break;
		}
		case PacketType::Disconnect:
		{
			if (header.ID != m_LocalID)
			{
				DisconnectPacket packet;
				memcpy(&packet, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(DisconnectPacket));
				for (auto& callback : m_ClientDisconnectCallbacks)
					callback(packet.ID);
				if (m_ClientData.find(header.ID) == m_ClientData.end())
					__debugbreak();
				m_ClientData.erase(packet.ID);
			}
			break;
		}
		case PacketType::Input:
		{
			if (header.ID != m_LocalID)
			{
				InputPacket packet;
				memcpy(&packet, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(InputPacket));
				
				auto& clientData = m_ClientData[header.ID];
				clientData.LatestInputUpdate = packet;
			}
			break;
		}
		case PacketType::TransformUpdate:
		{
			if (header.ID != m_LocalID)
			{
				TransformUpdatePacket packet;
				memcpy(&packet, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(TransformUpdatePacket));

				auto& clientData = m_ClientData[header.ID];
				clientData.LatestTransformUpdate = packet;
			}
			break;
		}
		case PacketType::PlayerUpdate:
		{
			if (header.ID != m_LocalID)
			{
				PlayerUpdatePacket packet;
				memcpy(&packet, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(PlayerUpdatePacket));

				auto& clientData = m_ClientData[header.ID];
				clientData.LatestPlayerUpdate = packet;
			}
			break;
		}
		case PacketType::PhysicsData:
		{
			if (header.ID != m_LocalID)
			{
				PhysicsDataPacket packet;
				memcpy(&packet, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(PhysicsDataPacket));

				auto& clientData = m_ClientData[header.ID];
				clientData.LatestPhysicsData = packet;
			}
			break;
		}
		case PacketType::EntityUpdate:
		{
			if (header.ID != m_LocalID)
			{
				EntityUpdatePacket packet;
				memcpy(&packet, static_cast<uint8*>(data + sizeof(PacketHeader)), sizeof(EntityUpdatePacket));
				for (auto& callback : m_EntityUpdateCallbacks)
					callback(packet);
			}
			break;
		}
		}
	}

}