#include "../io_socket.h"


#include <WinSock2.h>

// Link with ws2_32.lib
#pragma comment(lib, "ws2_32.lib")


void io_socket::init_server()
{
	/*
	// Initialize winSock library (v2.0)
	m_log.debug("Initializing winSock library (v2.0)...");
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,0), &wsaData) != NO_ERROR) {
		m_log.error("URemoteListener::InitServer(), WSAStartup() failed with error: " + std::to_string(WSAGetLastError()));
		WSACleanup();
		return false;
	}

	s_instanceCount++;

	// Create listener socket for incoming connections
	m_log.debug("Creating listener socket for incoming connections...");
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (m_listenSocket == INVALID_SOCKET) {
		m_log.error("URemoteListener::InitServer(), socket() failed with error: " + std::to_string(WSAGetLastError()));
		freeServer();
		return false;
	}

	// Socket thecnical info
	SOCKADDR_IN socketAddress; 
	socketAddress.sin_addr.s_addr	= htonl(INADDR_ANY); // Server address
	socketAddress.sin_family		= AF_INET; // Type of socket (AF_INET = Internet)
	socketAddress.sin_port			= htons(m_config->Port);

	// Bind the socket to the address and port defined in SOCKADDR
	m_log.debug("Binding the socket to the address and port...");
	if (bind(m_listenSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress)) == SOCKET_ERROR) {
		m_log.error("URemoteListener::InitServer(), bind() failed with error: " + std::to_string(WSAGetLastError()));
		freeServer();
		return false;
	}

	// Listen to incoming connections
	m_log.debug("Listen to incoming connections...");
	if (listen(m_listenSocket, m_config->MaxConcurrentConnections) == SOCKET_ERROR) {
		m_log.error("URemoteListener::InitServer(), listen() failed with error: " + std::to_string(WSAGetLastError()));
		freeServer();
		return false;
	}*/
}