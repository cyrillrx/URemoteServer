#include "winsock_helper.h"

#include "exception/Exception.h"

//////////////////////////////////////////////
// winsock_handler
//////////////////////////////////////////////

winsock_handler::winsock_handler()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2,0), &wsaData);
	winsock_helper::check_result("::winsock_handler", result);
}

winsock_handler::~winsock_handler()
{
	WSACleanup();
}

//////////////////////////////////////////////
// socket_handler
//////////////////////////////////////////////

socket_handler::socket_handler(SOCKET socket)
	: m_socket(socket) { }

socket_handler::~socket_handler()
{
	closesocket(m_socket);
}

SOCKET socket_handler::get_socket()
{
	return m_socket;
}

//////////////////////////////////////////////
// winsock_helper
//////////////////////////////////////////////

void winsock_helper::check_socket(const std::string& source, const SOCKET& socket)
{
	if (socket == INVALID_SOCKET) {
		throw Exception("winsock_exception", source, "Socket is invalid.");
	}
}

void winsock_helper::check_result(const std::string& source, const int& result)
{
	if (result != NO_ERROR) {
		throw Exception("winsock_exception", source, get_message(result));
	}
}

std::string winsock_helper::get_message(const int& result)
{
	std::string msg;

	switch(result) {
	case SOCKET_ERROR:
		return "Socket error.";

	default:
		return "Unknown : " + std::to_string(result);
	}
}