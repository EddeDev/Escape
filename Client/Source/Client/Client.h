#pragma once

#include "Core/Packet.h"

#include <enet/enet.h>

namespace esc {

	struct ClientCreateInfo
	{
		std::string Address = "127.0.0.1";
		uint16 Port = 42650;

		std::string Username;
	};

	struct ClientData
	{
		int32 ID = -1;
		std::string Username = "";
		TransformUpdatePacket LastTransformPacket;
	};

	class Client : public ReferenceCounted
	{
	public:
		Client(const ClientCreateInfo& createInfo);
		virtual ~Client();

		void Update();

		template<typename T>
		void SendPacket(PacketType type, const T& data)
		{
			if (!m_IsConnected)
				return;

			PacketHeader header;
			header.ID = m_LocalID;
			header.Type = type;

			uint32 bufferSize = sizeof(PacketHeader) + sizeof(T);
			uint8* buffer = new uint8[bufferSize];
			memcpy(buffer, &header, sizeof(PacketHeader));
			memcpy(static_cast<uint8*>(buffer + sizeof(PacketHeader)), &data, sizeof(T));

			ENetPacket* packet = enet_packet_create(buffer, bufferSize, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(m_Peer, 0, packet);

			delete[] buffer;
		}

		std::map<uint32, ClientData>& GetClientData() { return m_ClientData; }
		const std::map<uint32, ClientData>& GetClientData() const { return m_ClientData; }
	private:
		void ParsePacketData(uint8* data);
	private:
		ClientCreateInfo m_CreateInfo;
		ENetHost* m_Client = nullptr;
		ENetPeer* m_Peer = nullptr;
		bool m_IsConnected = false;

		int32 m_LocalID = -1;
		std::map<uint32, ClientData> m_ClientData;
	};

}