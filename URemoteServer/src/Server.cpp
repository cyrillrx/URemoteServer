#include "Server.h"

#include <iostream>
#include <comdef.h>
#include "StringUtils.h"
#include "Exchange.h"

//#define BUFFER_SIZE BUFSIZ
#define BUFFER_SIZE 4096

using namespace std;
using namespace network;

int Server::s_instanceCount = 0;

//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////

Server::Server(unique_ptr<ServerConfig> config, AI* ai) : m_config(move(config))
{	
	m_Ai = ai;
	m_log = new Logger("Server.log");
	InitServer();
	// TODO: Create inner logger and keep log out of console
}

Server::~Server(void)
{
	if (m_log) {
		delete(m_log);
		m_log = nullptr;
	}
	FreeServer();
}

/** 
 * Launch the server 
 * @return true if everything went correctly. False otherwise
 */
void Server::Start()
{
	m_log->info("******************************************************");
	m_log->info("*****               Server Started               *****");
	m_log->info("******************************************************");

	m_continueToListen = true;

	char buffer[BUFFER_SIZE];

	/* connection socket */
	
	while (m_continueToListen) {
		m_log->debug("Server Info : ");
		m_log->debug(" - Hostname   : " + m_hostname);
		m_log->debug(" - IP Address : " + m_ipAddress);
		m_log->debug(" - Open port  : " + to_string(m_config->Port));
		m_log->debug("Waiting for client to connect...");
		
		m_cSocket = accept(m_listenSocket, nullptr, nullptr);
		if (m_cSocket == INVALID_SOCKET) {
			m_log->error("accept() failed with error: " + to_string(WSAGetLastError()));
			FreeServer();
			return;
		}
		
		StringUtils::clear_buffer(buffer);

		int received = recv(m_cSocket, buffer, sizeof(buffer), 0);
		m_log->debug("  -- result : " + received);

		SerializedExchange exchange;
		exchange.buffer = buffer;
		exchange.bufferSize = received;

		HandleMessage(exchange);

		closesocket(m_cSocket);
		m_log->debug("Socket closed.");
	}
}

/** Stop the server listening loop. */
bool Server::Stop()
{
	m_continueToListen = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////////////////////

/**
 * Initialize the server.
 */
bool Server::InitServer() 
{
	// Initialize winSock library (v2.0)
	m_log->debug("Initializing winSock library (v2.0)...");
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,0), &wsaData) != NO_ERROR) {
		m_log->error("Server::InitServer(), WSAStartup() failed with error: " + to_string(WSAGetLastError()));
		WSACleanup();
		return false;
	}
	
	s_instanceCount++;

	// Create listener socket for incoming connections
	m_log->debug("Creating listener socket for incoming connections...");
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (m_listenSocket == INVALID_SOCKET) {
		m_log->error("Server::InitServer(), socket() failed with error: " + to_string(WSAGetLastError()));
		FreeServer();
		return false;
	}

	// Socket thecnical info
	SOCKADDR_IN socketAddress; 
	socketAddress.sin_addr.s_addr	= htonl(INADDR_ANY); // Server address
	socketAddress.sin_family		= AF_INET; // Type of socket (AF_INET = Internet)
	socketAddress.sin_port			= htons(m_config->Port);

	// Bind the socket to the address and port defined in SOCKADDR
	m_log->debug("Binding the socket to the address and port...");
	if (bind(m_listenSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress)) == SOCKET_ERROR) {
		m_log->error("Server::InitServer(), bind() failed with error: " + to_string(WSAGetLastError()));
		FreeServer();
		return false;
	}

	// Listen to incoming connections
	m_log->debug("Listen to incoming connections...");
	if (listen(m_listenSocket, m_config->MaxConcurrentConnections) == SOCKET_ERROR) {
		m_log->error("Server::InitServer(), listen() failed with error: " + to_string(WSAGetLastError()));
		FreeServer();
		return false;
	}

	m_hostname = GetHostName();
	m_ipAddress = GetIpAddress(m_hostname);

	return true;
}

/** Free the sockets. */
void Server::FreeServer()
{
	closesocket(m_listenSocket);
		
	s_instanceCount--;
	if (s_instanceCount < 1) {
		WSACleanup();
	}
}


string Server::GetHostName()
{
	char hostname[80];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		m_log->error("Error " + to_string(WSAGetLastError()) + " when getting local host name.");
		return "";
	}

	return hostname;
}

string Server::GetIpAddress(string hostname)
{
	string ipAddress = "";
	struct hostent *host = gethostbyname(hostname.c_str());
	if (host == 0) {
		m_log->error("Yow! Bad host lookup.");
	}

	struct in_addr addr;
	memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
	ipAddress = inet_ntoa(addr);

	return ipAddress;
}

/**
 * Handle the command sent by the client.
 * then send a response.
 */
void Server::HandleMessage(SerializedExchange request) 
{
	SerializedExchange response = Exchange::HandleMessage(m_Ai, request, m_continueToListen);
	send(m_cSocket, response.buffer, response.bufferSize, 0);
}