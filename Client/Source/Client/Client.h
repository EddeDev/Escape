#pragma once

#include "Core/TCPClient.h"

namespace esc {

	struct ClientCreateInfo
	{
		std::string Address = "127.0.0.1";
		uint16 Port = 42650;
	};

	class TCP;

	class Client : public ReferenceCounted
	{
	public:
		Client(const ClientCreateInfo& createInfo);
		virtual ~Client();

		const ClientCreateInfo& GetCreateInfo() const { return m_CreateInfo; }

		static Client& Get() { return *s_Instance; }
	private:
		inline static Client* s_Instance = nullptr;

		ClientCreateInfo m_CreateInfo;
		TCP* m_TCP = nullptr;
	};

	class TCP
	{
	public:
		TCP() = default;

		void Connect();
	private:
		void ConnectCallback(const AsyncResult& result);
		void ReceiveCallback(const AsyncResult& result);
	private:
		Ref<TcpClient> m_Socket;
		Ref<NetworkStream> m_Stream;

		uint8* m_ReceiveBuffer = nullptr;
	};

}