#pragma once

//#include "platform_config.h"
//# if defined(WINDOWS_PLATFORM)
//#	include "windows/network_io.h"
//# else
//#	include "unix/network_io.h"
//# endif

#include <string>

namespace network_io
{
	std::string hostname();
	std::string ip_address(const std::string& hostname);
}

