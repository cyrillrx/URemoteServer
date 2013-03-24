#include "URemoteListener.h"

#include <iostream>
#include <WinSock2.h>

#include "string_utils.h"
#include "io_socket.h"
#include "exception/Exception.h"
#include "../Exchange.h"

// Link with ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

//#define BUFFER_SIZE BUFSIZ
#define BUFFER_SIZE 4096

using namespace network;

int URemoteListener::s_instanceCount = 0;

//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////

URemoteListener::URemoteListener(std::unique_ptr<ServerConfig> config, AI* ai)
	: m_config(move(config)), m_ai(ai)
{	
	m_log = logger("URemoteListener.log");
	m_log.setLogSeverityConsole(logger::SEVERITY_LVL_WARNING);

	m_log.info("******************************************************");
	m_log.info("*****                URemoteListener             *****");
	m_log.info("******************************************************");

	// Init hostname
	try {
		m_hostname	= io_socket::hostname();
	} catch (const Exception& e) {
		m_hostname = "localhost";
		m_log.error("URemoteListener::InitServer(), " + e.whatAsString());
	}

	// Init ip address
	try {
		m_ipAddress	= io_socket::ip_address(m_hostname);
	} catch (const Exception& e) {
		m_ipAddress = "127.0.0.1";
		m_log.error("URemoteListener::InitServer(), " + e.whatAsString());
	}

	initServer();
}

URemoteListener::~URemoteListener()
{
	m_log.info("******************************************************");
	m_log.info("*****               ~URemoteListener             *****");
	m_log.info("******************************************************");

	freeServer();
}

/** 
* Launch the server 
* @return true if everything went correctly. False otherwise
*/
void URemoteListener::doStart()
{
	m_log.info("Do start");
	m_continueToListen = true;

	char buffer[BUFFER_SIZE];

	/* connection socket */

	while (m_continueToListen) {
		m_log.debug("Server Info : ");
		m_log.debug(" - Hostname   : " + m_hostname);
		m_log.debug(" - IP Address : " + m_ipAddress);
		m_log.debug(" - Open port  : " + std::to_string(m_config->Port));
		m_log.debug("Waiting for client to connect...");

		m_cSocket = ::accept(m_listenSocket, nullptr, nullptr);
		if (m_cSocket == INVALID_SOCKET) {
			m_log.error("accept() failed with error: " + std::to_string(WSAGetLastError()));
			freeServer();
			return;
		}

		string_utils::clear_buffer(buffer);

		int received = recv(m_cSocket, buffer, sizeof(buffer), 0);
		m_log.debug("  -- result : " + received);

		SerializedExchange exchange;
		exchange.buffer = buffer;
		exchange.bufferSize = received;

		handleMessage(exchange);

		closesocket(m_cSocket);
		m_log.debug("Socket closed.");
	}
}

//////////////////////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////////////////////

/**
* Initialize the server.
*/
bool URemoteListener::initServer() 
{
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
	if (::bind(m_listenSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress)) == SOCKET_ERROR) {
		m_log.error("URemoteListener::InitServer(), bind() failed with error: " + std::to_string(WSAGetLastError()));
		freeServer();
		return false;
	}

	// Listen to incoming connections
	m_log.debug("Listen to incoming connections...");
	if (::listen(m_listenSocket, m_config->MaxConcurrentConnections) == SOCKET_ERROR) {
		m_log.error("URemoteListener::InitServer(), listen() failed with error: " + std::to_string(WSAGetLastError()));
		freeServer();
		return false;
	}

	return true;
}

/** Free the sockets. */
void URemoteListener::freeServer()
{
	closesocket(m_listenSocket);

	s_instanceCount--;
	if (s_instanceCount < 1) {
		WSACleanup();
	}
}

/**
* Handle the command sent by the client.
* then send a response.
*/
void URemoteListener::handleMessage(SerializedExchange request) 
{
	SerializedExchange response = Exchange::handleMessage(m_ai, request);
	send(m_cSocket, response.buffer, response.bufferSize, 0);
}

//TODO: Define a function (inline) for std::to_string(WSAGetLastError())