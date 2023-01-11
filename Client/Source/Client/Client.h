#pragma once

namespace esc {

	struct ClientCreateInfo
	{
		std::string Address;
		uint16 Port;

		bool KeepAlive = false;
		bool NoDelay = true;
		bool Blocking = false;
		bool ReuseAddress = false;
		uint32 Timeout = 100;
		uint8 TrafficClass = 24;
	};

	class Client : public ReferenceCounted
	{
	public:
		Client(const ClientCreateInfo& createInfo);
		virtual ~Client();
	private:
		uint64 m_ConnectSocket = static_cast<uint64>(~0);
	};

}