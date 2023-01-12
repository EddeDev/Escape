#include "ServerPCH.h"
#include "Client.h"

namespace esc {

	static const uint32 s_DataBufferSize = 4096;

	Client::Client(uint32 id)
		: m_ID(id)
	{
		m_TCP = new TCP(id);
	}

	Client::~Client()
	{
		delete m_TCP;
	}

	TCP::TCP(uint32 id)
		: m_ID(id)
	{
	}

	void TCP::Connect(Ref<TcpClient> socket)
	{
		m_Socket = socket;
		m_Socket->SetReceiveBufferSize(s_DataBufferSize);
		m_Socket->SetSendBufferSize(s_DataBufferSize);

		m_Stream = m_Socket->GetStream();

		m_ReceiveBuffer = new uint8[s_DataBufferSize];
		memset(m_ReceiveBuffer, 0, s_DataBufferSize);

		m_Stream->BeginRead(m_ReceiveBuffer, 0, s_DataBufferSize, ESCAPE_BIND_CALLBACK(ReceiveCallback, this), nullptr);
	}

	void TCP::ReceiveCallback(const AsyncResult& result)
	{
		int32 bufferSize = m_Stream->EndRead(result);
		if (bufferSize <= 0)
			return;

		uint8* buffer = new uint8[bufferSize];
		memcpy(buffer, m_ReceiveBuffer, bufferSize);

		m_Stream->BeginRead(m_ReceiveBuffer, 0, s_DataBufferSize, ESCAPE_BIND_CALLBACK(ReceiveCallback, this), nullptr);
	}

}