#include "ClientPCH.h"
#include "EscapeGame.h"

#define ESCAPE_TEST_ADDRESS "127.0.0.1"
#define ESCAPE_DEFAULT_PORT 42650

namespace esc {

	void Run()
	{
		EscapeGame* instance = new EscapeGame(ESCAPE_TEST_ADDRESS, ESCAPE_DEFAULT_PORT);
		instance->Run();
		delete instance;
	}

}

int main()
{
	esc::Run();
	return 0;
}