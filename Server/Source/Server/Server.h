#pragma once

namespace esc {

	class Server
	{
	public:
		Server(uint16 port);
		~Server();

		void Run();
	private:
		uint64 m_ListenSocket = static_cast<uint64>(~0);
		uint64 m_ClientSocket = static_cast<uint64>(~0);
	};

}