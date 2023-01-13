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
		TransformUpdate Transform;
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
#if 0
			char buffer[512];
			memset(buffer, 0, 512);

			buffer[0] = (char)type;
			buffer[1] = '|';

			strcat(buffer, (const char*)&data);
#endif

#if 0
			uint8* buffer = new uint8[sizeof(T) + 4];
			memcpy(&buffer[0], &type, sizeof(uint32));
			memcpy(&buffer[4], &data, sizeof(T));

			ENetPacket* packet = enet_packet_create(buffer, sizeof(T) + 4, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(m_Peer, 0, packet);
#endif

			uint32 bufferSize = sizeof(T) + 2;

			uint8* buffer = new uint8[bufferSize];
			memset(buffer, 0, bufferSize);

			std::string typeStr = std::to_string((uint32)type);
			buffer[0] = typeStr[0];
			buffer[1] = char('|');
			memcpy(&buffer[2], &data, sizeof(T));

			ENetPacket* packet = enet_packet_create(buffer, bufferSize, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(m_Peer, 0, packet);
		}

		std::map<uint32, ClientData>& GetClientData() { return m_ClientData; }
		const std::map<uint32, ClientData>& GetClientData() const { return m_ClientData; }
	private:
		void ParsePacketData(uint8* data);
	private:
		ClientCreateInfo m_CreateInfo;
		ENetHost* m_Client = nullptr;
		ENetPeer* m_Peer = nullptr;

		int32 m_LocalID = -1;
		std::map<uint32, ClientData> m_ClientData;
	};

}