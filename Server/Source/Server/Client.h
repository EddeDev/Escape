#pragma once

#include "Core/TCPClient.h"

namespace esc {

	class TCP;

	class Client : public ReferenceCounted
	{
	public:
		Client(uint32 id);
		virtual ~Client();

		TCP* GetTCP() const { return m_TCP; }
	private:
		uint32 m_ID = 0;
		TCP* m_TCP = nullptr;
	};

	class TCP
	{
	public:
		TCP(uint32 id);

		void Connect(Ref<TcpClient> socket);

		Ref<TcpClient> GetSocket() const { return m_Socket; }
	private:
		void ReceiveCallback(const AsyncResult& result);
	private:
		uint32 m_ID = 0;
		Ref<TcpClient> m_Socket;
		Ref<NetworkStream> m_Stream;

		uint8* m_ReceiveBuffer = nullptr;
	};

}