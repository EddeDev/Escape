#pragma once

namespace esc {

	class Client : public ReferenceCounted
	{
	public:
		Client(const std::string& address, uint16 port);
		virtual ~Client();
	private:
		uint64 m_ConnectSocket = static_cast<uint64>(~0);
	};

}