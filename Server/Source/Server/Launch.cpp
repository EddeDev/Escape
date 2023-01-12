#include "ServerPCH.h"

#include "Server.h"

#include <Windows.h>

namespace esc {

	void Main()
	{
		SetConsoleTitleW(L"Server");

		Server::Start(20, 42650);

		std::cin.get();
	}

}

int main()
{
	esc::Main();
	return 0;
}