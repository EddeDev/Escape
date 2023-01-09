#include "EscapePCH.h"
#include "EscapeGame.h"

namespace Escape {

	void Run()
	{
		EscapeGame* instance = new EscapeGame();
		instance->Run();
		delete instance;
	}

}

int main()
{
	Escape::Run();
	return 0;
}