#pragma once

#include "platform_config.h"
# if defined(WINDOWS_PLATFORM)
#	include "windows/io_socket.h"
# endif

#include <string>

namespace io_socket
{
	void connection();
	std::string hostname();
	std::string ip_address(const std::string& hostname);
}

