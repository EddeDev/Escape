#include "ServerPCH.h"
#include "Server.h"

#define ESCAPE_TEST_ADDRESS "127.0.0.1"
#define ESCAPE_DEFAULT_PORT 42650

namespace esc {

	void Run()
	{
		Server* instance = new Server(ESCAPE_DEFAULT_PORT);
		instance->Run();
		delete instance;
	}

}

int main()
{
	esc::Run();
	return 0;
}