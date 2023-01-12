#pragma once

#include <enet/enet.h>

namespace esc {

	struct ClientCreateInfo
	{
		std::string Address = "127.0.0.1";
		uint16 Port = 42650;

		std::string Username;
	};

	class Client : public ReferenceCounted
	{
	public:
		Client(const ClientCreateInfo& createInfo);
		virtual ~Client();

		void Update();
	private:
		void ParsePacketData(uint8* data);
	private:
		ClientCreateInfo m_CreateInfo;
		ENetHost* m_Client = nullptr;
		ENetPeer* m_Peer = nullptr;

		int32 m_ID = -1;
	
		struct ClientData
		{
			uint32 ID;
			std::string Username;
		};
		std::map<uint32, ClientData> m_ClientData;
	};

}