#include "ClientPCH.h"
#include "EscapeGame.h"

namespace esc {

	void Run()
	{
#ifdef ESCAPE_BUILD_DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif

		std::cout << "IP Address: ";
		std::string address;
#if ESCAPE_ONLINE
		std::cin >> address;
#else
		address = ESCAPE_TEST_ADDRESS;
		std::cout << address << std::endl;
#endif

		std::cout << "Username: ";
		std::string username;
#if ESCAPE_ONLINE
		std::cin >> username;
#else
		username = "EddeDev";
		std::cout << username << std::endl;
#endif

		EscapeGame* instance = new EscapeGame(address, ESCAPE_DEFAULT_PORT, username);
		instance->Run();
		delete instance;
	}

}

int main()
{
	esc::Run();
	return 0;
}