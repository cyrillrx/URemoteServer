#pragma once

#include "platform_config.h"
#if defined(WINDOWS_PLATFORM)

#include <string>
#include <WinSock2.h>

class winsock_handler
{
public:
	winsock_handler();
	~winsock_handler();
};

class socket_handler
{
public:
	socket_handler(SOCKET m_socket);
	~socket_handler();

	SOCKET get_socket();

private:
	SOCKET m_socket;
};

namespace winsock_helper
{
	void check_socket(const std::string& source, const SOCKET& socket);
	void check_result(const std::string& source, const int& result);
	std::string get_message(const int& result);
}

#endif
