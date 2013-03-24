/*#include "../io_socket.h"

#include "Utils.h"
#include "helpers/winsock_helper.h"
#include <WinSock2.h>

// Link with ws2_32.lib
#pragma comment(lib, "ws2_32.lib")


void io_socket::init_server()
{
	//TODO: Change declaration of "port"
	unsigned short port = 1234;
	int max_concurrent_connections = 3;

	// Initialize winSock library (v2.0)
	Utils::getLogger()->debug("Initializing winSock library (v2.0)...");
	winsock_handler initLibrary;

	// Create listener socket for incoming connections
	Utils::getLogger()->debug("Creating listener socket for incoming connections...");
	socket_handler listenSocket(::socket(AF_INET, SOCK_STREAM, IPPROTO_IP));
	winsock_helper::check_socket("io_socket::init_server, creating listenSocket", listenSocket.get_socket());
	//m_log.error("URemoteListener::InitServer(), socket() failed with error: " + std::to_string(WSAGetLastError()));

	// Socket thecnical info
	SOCKADDR_IN socketAddress; 
	socketAddress.sin_addr.s_addr	= htonl(INADDR_ANY); // Server address
	socketAddress.sin_family		= AF_INET; // Type of socket (AF_INET = Internet)
	socketAddress.sin_port			= htons(port);

	// Bind the socket to the address and port defined in SOCKADDR
	Utils::getLogger()->debug("Binding the socket to the address and port...");
	int result = ::bind(listenSocket.get_socket(), (SOCKADDR*)&socketAddress, sizeof(socketAddress));
	winsock_helper::check_result("io_socket::init_server, bind listenSocket", result);
	//m_log.error("URemoteListener::InitServer(), bind() failed with error: " + std::to_string(WSAGetLastError()));

	// Listen to incoming connections
	Utils::getLogger()->debug("Listen to incoming connections...");
	result = ::listen(listenSocket.get_socket(), max_concurrent_connections);
	winsock_helper::check_result("io_socket::init_server, listen listenSocket", result);
	// m_log.error("URemoteListener::InitServer(), listen() failed with error: " + std::to_string(WSAGetLastError()));

	socket_handler connectionSocket(::accept(listenSocket.get_socket(), nullptr, nullptr));
	winsock_helper::check_socket("io_socket::init_server, accept connection", connectionSocket.get_socket());
}*/