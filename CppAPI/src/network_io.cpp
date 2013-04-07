#include "network_io.h"

#include "platform_config.h"
# if defined(WINDOWS_PLATFORM)
#   include <WinSock2.h>
# endif

#include <boost/asio/ip/host_name.hpp>
#include "exception/socket_exception.h"

namespace network_io {

	std::string hostname()
	{
		try {
			return boost::asio::ip::host_name();

		} catch (const boost::system::error_code& e) {
			throw socket_exception("network_io::hostname()", e.message());
		}
	}

	std::string ip_address(const std::string& hostname)
	{
		auto host = gethostbyname(hostname.c_str());
		if (host == 0) {
			throw socket_exception("network_io::ip_address()", "Bad host lookup.");
		}

		struct in_addr addr;
		memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));

		return inet_ntoa(addr);
	}
}