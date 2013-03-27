#include "network_io.h"

//#include <boost/asio.hpp>
#include <boost/asio/ip/host_name.hpp>
#include "exception/socket_exception.h"

//#include <boost/asio/ip/tcp.hpp>
//#include <boost/bind.hpp>

void network_io::connection()
{
	/*
	unsigned short port = 1234;
	using boost::asio::ip::tcp;
	boost::asio::io_service service;
	tcp::socket socket(service);
	
	tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), port));
	acceptor.accept(socket);
	*/
	//boost::bind

		// init listener socket
		// socket
		// Bind
		// listen

		// connection socket
		// csoc = accept(listernerSock


}

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