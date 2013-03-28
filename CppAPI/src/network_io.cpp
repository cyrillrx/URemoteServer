#include "network_io.h"

#include <boost/asio/ip/host_name.hpp>
#include "exception/socket_exception.h"

std::string network_io::hostname()
{
	try {
		return boost::asio::ip::host_name();

	} catch (const boost::system::error_code& e) {
		throw socket_exception("network_io::hostname()", e.message());
	}
}

std::string network_io::ip_address(const std::string& hostname)
{
	std::string ipAddress = "";
	struct hostent *host = gethostbyname(hostname.c_str());
	if (host == 0) {
		throw socket_exception("network_io::ip_address()", "Bad host lookup.");
	}

	struct in_addr addr;
	memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
	ipAddress = inet_ntoa(addr);

	return ipAddress;
}