#include "ClientPCH.h"
#include "Client.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace esc {

	Client::Client(const ClientCreateInfo& createInfo)
	{
		std::cout << "Initializing client..." << std::endl;

		WSAData wsaData = {};
		if (int32 result = WSAStartup(MAKEWORD(2, 2), &wsaData); result != 0)
		{
			std::cerr << "WSAStartup failed with error: " << result << std::endl;
			__debugbreak();
			return;
		}

		addrinfo hints = {};
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		std::string portString = std::to_string(createInfo.Port);

		addrinfo* info = NULL;
		if (int32 result = getaddrinfo(createInfo.Address.c_str(), portString.c_str(), &hints, &info); result != 0)
		{
			std::cerr << "getaddrinfo failed with error: " << result << std::endl;
			__debugbreak();
			return;
		}

		for (addrinfo* infoPtr = info; infoPtr != nullptr; infoPtr = infoPtr->ai_next)
		{
			m_ConnectSocket = socket(infoPtr->ai_family, infoPtr->ai_socktype, infoPtr->ai_protocol);
			if (m_ConnectSocket == INVALID_SOCKET)
			{
				std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
				__debugbreak();
				return;
			}

			int32 result = connect(m_ConnectSocket, infoPtr->ai_addr, (int32)infoPtr->ai_addrlen);
			if (result == SOCKET_ERROR)
			{
				closesocket(m_ConnectSocket);
				m_ConnectSocket = INVALID_SOCKET;
				continue;
			}

			break;
		}

		freeaddrinfo(info);

		if (m_ConnectSocket == INVALID_SOCKET)
		{
			std::cout << "Unable to connect to server! (" << createInfo.Address << ":" << portString << ")" << std::endl;
			__debugbreak();
			return;
		}

#define SET_OPTION(level, name, value, type) \
		{ \
			if (int32 result = setsockopt(m_ConnectSocket, level, name, (const char*)&value, sizeof(type)); result != 0) \
			{ \
				std::cout << "setsockopt(" << m_ConnectSocket << ", " << #level << ", " << #name << ", " << (void*)&value << ", " << sizeof(type) << ") failed with error: " << WSAGetLastError() << std::endl; \
				__debugbreak(); \
				return; \
			} \
		} \

		uint32 keepAlive = createInfo.KeepAlive ? 1 : 0;
		uint8 trafficClass = createInfo.TrafficClass;
		uint32 reuseAddress = createInfo.ReuseAddress ? 1 : 0;
		uint32 noDelay = createInfo.NoDelay ? 1 : 0;
		SET_OPTION(IPPROTO_TCP, TCP_NODELAY, noDelay, uint32);
		SET_OPTION(IPPROTO_IP, IP_TOS, trafficClass, uint8);
		SET_OPTION(SOL_SOCKET, SO_KEEPALIVE, reuseAddress, uint32);
		SET_OPTION(SOL_SOCKET, SO_REUSEADDR, noDelay, uint32);

		timeval timeout = {};
		timeout.tv_usec = createInfo.Timeout * 1000;
		SET_OPTION(SOL_SOCKET, SO_RCVTIMEO, timeout, timeval);
	
		unsigned long nonBlocking = 1;
		unsigned long blocking = 0;
		ioctlsocket(m_ConnectSocket, FIONBIO, createInfo.Blocking ? &blocking : &nonBlocking);

		std::cout << "Successfully connected to server! (" << createInfo.Address << ":" << portString << ")" << std::endl;
	}

	Client::~Client()
	{
		closesocket(m_ConnectSocket);
		WSACleanup();
	}

}