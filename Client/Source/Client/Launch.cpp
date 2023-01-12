#include "ClientPCH.h"
#include "EscapeGame.h"

#define ESCAPE_TEST_ADDRESS "127.0.0.1"
#define ESCAPE_DEFAULT_PORT 42650

namespace esc {

	void Run()
	{
#ifdef ESCAPE_BUILD_DEBUG
		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#endif

		std::cout << "Username: ";
		std::string username;
		std::cin >> username;

		EscapeGame* instance = new EscapeGame(ESCAPE_TEST_ADDRESS, ESCAPE_DEFAULT_PORT, username);
		instance->Run();
		delete instance;

#ifdef ESCAPE_BUILD_DEBUG
		_CrtDumpMemoryLeaks();
#endif
	}

}

int main()
{
	esc::Run();
	return 0;
}