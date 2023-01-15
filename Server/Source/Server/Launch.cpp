#include "ServerPCH.h"

#include "Server.h"
#include "Time.h"
#include "Sync.h"

#include <Windows.h>

namespace esc {

	void Main()
	{
		SetConsoleTitleW(L"Server");

		Time::Init();
		Sync::Init();

		ServerCreateInfo serverCreateInfo;
		serverCreateInfo.MaxPlayers = ESCAPE_MAX_PLAYERS;
		serverCreateInfo.Port = ESCAPE_DEFAULT_PORT;

		Server* server = new Server(serverCreateInfo);

		float lastTime = 0.0f;
		uint32 frames = 0;

		bool isCloseRequested = false;
		while (!isCloseRequested)
		{
			frames++;
			while (Time::Get() >= lastTime + 1.0f)
			{
				// std::cout << frames << " fps" << std::endl;
				lastTime += 1.0f;
				frames = 0;
			}

#if 0
			Sync::Tick(static_cast<int32>(1.0f / ESCAPE_FIXED_TIMESTEP));
#else
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(5ms);
#endif
			server->Update();
		}

		delete server;

		Sync::Shutdown();
	}

}

int main()
{
	esc::Main();
	return 0;
}