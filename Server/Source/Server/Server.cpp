#include "ServerPCH.h"
#include "Server.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace esc {

	namespace Utils {

		void* GetInAddr(sockaddr* addr)
		{
			if (addr->sa_family == AF_INET)
				return &(((sockaddr_in*)addr)->sin_addr);
			return &(((sockaddr_in6*)addr)->sin6_addr);
		}

	}

	Server::Server(uint16 port)
	{
		SetConsoleTitleA("Server");
		std::cout << "Starting server..." << std::endl;

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
		if (int32 result = getaddrinfo(NULL, portString.c_str(), &hints, &info); result != 0)
		{
			std::cerr << "getaddrinfo failed with error: " << result << std::endl;
			__debugbreak();
			return;
		}

		m_ListenSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
		if (m_ListenSocket == INVALID_SOCKET)
		{
			std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
			__debugbreak();
			return;
		}

		if (int32 result = bind(m_ListenSocket, info->ai_addr, (int32)info->ai_addrlen); result != 0)
		{
			std::cerr << "bind failed with error: " << result << std::endl;
			__debugbreak();
			return;
		}

		freeaddrinfo(info);

		std::cout << "Server started on " << portString << "." << std::endl;

		if (int32 result = listen(m_ListenSocket, SOMAXCONN); result == SOCKET_ERROR)
		{
			std::cerr << "listen failed with error: " << result << std::endl;
			__debugbreak();
			return;
		}

		m_ClientSocket = accept(m_ListenSocket, NULL, NULL);
		if (m_ClientSocket == INVALID_SOCKET)
		{
			std::cerr << "accept failed with error: " << WSAGetLastError() << std::endl;
			__debugbreak();
			return;
		}

		std::cout << "Closing listening socket..." << std::endl;
		closesocket(m_ListenSocket);

		sockaddr clientInfo = {};
		int32 addrSize = sizeof(sockaddr);
		getpeername(m_ClientSocket, &clientInfo, &addrSize);
		
		in_addr& inAddr = *(&(((sockaddr_in*)&clientInfo)->sin_addr));
		char* addressString = inet_ntoa(inAddr);

		std::cout << "Incoming connection from " << addressString << std::endl;

		while (true)
		{

		}
	}

	Server::~Server()
	{
		if (shutdown(m_ClientSocket, SD_SEND) == SOCKET_ERROR)
		{
			__debugbreak();
			return;
		}

		closesocket(m_ClientSocket);
		WSACleanup();
	}

	void Server::Run()
	{

	}

}