#include "ClientPCH.h"
#include "Client.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace esc {

	Client::Client(const std::string& address, uint16 port)
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

		std::string portString = std::to_string(port);

		addrinfo* info = NULL;
		if (int32 result = getaddrinfo(address.c_str(), portString.c_str(), &hints, &info); result != 0)
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
			std::cout << "Unable to connect to server! (" << address << ":" << portString << ")" << std::endl;
			// __debugbreak();
			return;
		}

		std::cout << "Successfully connected to server! (" << address << ":" << portString << ")" << std::endl;
	}

	Client::~Client()
	{

	}

}