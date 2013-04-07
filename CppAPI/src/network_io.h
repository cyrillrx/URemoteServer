#pragma once

#include <string>

namespace network_io
{
	std::string hostname();
	std::string ip_address(const std::string& hostname);
}

