#include "ClientPCH.h"
#include "Client.h"

namespace esc {

	static const uint32 s_DataBufferSize = 4096;

	Client::Client(const ClientCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		m_TCP = new TCP();
	}

	Client::~Client()
	{
		delete m_TCP;
	}

	void TCP::Connect()
	{
		m_Socket = Ref<TcpClient>::Create();
		m_Socket->SetReceiveBufferSize(s_DataBufferSize);
		m_Socket->SetSendBufferSize(s_DataBufferSize);

		m_ReceiveBuffer = new uint8[s_DataBufferSize];
		memset(m_ReceiveBuffer, 0, s_DataBufferSize);
		m_Socket->BeginConnect(Client::Get().GetCreateInfo().Address, Client::Get().GetCreateInfo().Port, ESCAPE_BIND_CALLBACK(ConnectCallback, this), &m_Socket);
	}

	void TCP::ConnectCallback(const AsyncResult& result)
	{
		m_Socket->EndConnect(result);

		if (!m_Socket->IsConnected())
			return;

		m_Stream = m_Socket->GetStream();
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