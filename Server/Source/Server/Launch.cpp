#include "ServerPCH.h"

#include "Server.h"
#include "Time.h"

#include <Windows.h>

namespace esc {

	void Main()
	{
		SetConsoleTitleW(L"Server");

		Time::Init();

		ServerCreateInfo serverCreateInfo;
		serverCreateInfo.MaxPlayers = 20;
		serverCreateInfo.Port = 42650;

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

			server->Update();

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(5ms);
		}

		delete server;
	}

}

int main()
{
	esc::Main();
	return 0;
}