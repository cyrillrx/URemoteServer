#include "Server.h"

#include <iostream>
#include <comdef.h>
#include "StringUtils.h"
#include "Exchange.h"

//#define BUFFER_SIZE BUFSIZ
#define BUFFER_SIZE 4096

using namespace std;
using namespace network;

int Server::s_InstanceCount = 0;

//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////

Server::Server(unique_ptr<ServerConfig> config, AI* ai) : m_Config(move(config))
{	
	m_Ai = ai;
	InitServer();
}

Server::~Server(void)
{
	FreeServer();
}

/** 
 * Launch the server 
 * @return true if everything went correctly. False otherwise
 */
bool Server::Start()
{
	m_ContinueToListen = true;

	char buffer[BUFFER_SIZE];

	/* connection socket */
	
	while (m_ContinueToListen) {
		cout << "Server Info : " << endl;
		cout << " - Hostname   : " << m_Hostname << endl;
		cout << " - IP Address : " << m_IpAddress << endl;
		cout << " - Open port  : " << m_Config->Port << endl;
		cout << "Waiting for client to connect..." << endl;
		
		//m_CSocket = accept(m_ListenSocket, (SOCKADDR *)&csin, &sizeofcsin);
		m_CSocket = accept(m_ListenSocket, nullptr, nullptr);
		if (m_CSocket == INVALID_SOCKET) {
			cerr << "accept failed with error: " << WSAGetLastError() << endl;
			FreeServer();
			return false;
		}
		
		StringUtils::ClearBuffer(buffer);

		int received = recv(m_CSocket, buffer, sizeof(buffer), 0);
		cout << "  -- result : " << received <<  endl << endl;

		SerializedExchange exchange;
		exchange.buffer = buffer;
		exchange.bufferSize = received;

		HandleMessage(exchange);

		closesocket(m_CSocket);
		cout << "Socket closed" << endl << endl;
	}

	return true;
}

/** Stop the server listening loop. */
bool Server::Stop()
{
	m_ContinueToListen = false;
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
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,0), &wsaData) != NO_ERROR) {
		cerr << "WSAStartup failed with error: " << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}
	
	s_InstanceCount++;

	// Create listener socket for incoming connections
	m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (m_ListenSocket == INVALID_SOCKET) {
		cerr << "socket failed with error: " << WSAGetLastError() << endl;
		FreeServer();
		return false;
	}

	// Socket thecnical info
	SOCKADDR_IN socketAddress; 
	socketAddress.sin_addr.s_addr	= htonl(INADDR_ANY); // Server address
	socketAddress.sin_family		= AF_INET; // Type of socket => Internet
	socketAddress.sin_port			= htons(m_Config->Port);

	// Bind the socket to the address and port defined in SOCKADDR
	if (bind(m_ListenSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress)) == SOCKET_ERROR) {
		cerr << "bind failed with error: " << WSAGetLastError() << endl;
		FreeServer();
		return false;
	}

	// Listen to incoming connections
	if (listen(m_ListenSocket, m_Config->MaxConcurrentConnections) == SOCKET_ERROR) {
		cerr << "listen failed with error: " << WSAGetLastError() << endl;
		FreeServer();
		return false;
	}

	m_Hostname = GetHostName();
	m_IpAddress = GetIpAddress(m_Hostname);

	return true;
}

/** Free the sockets. */
void Server::FreeServer()
{
	closesocket(m_ListenSocket);
		
	s_InstanceCount--;
	if (s_InstanceCount < 1) {
		WSACleanup();
	}
}


string Server::GetHostName()
{
	char hostname[80];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		cerr << "Error " << WSAGetLastError() << " when getting local host name." << endl;
		return "";
	}

	return hostname;
}

string Server::GetIpAddress(string hostname)
{
	string ipAddress = "";
	struct hostent *host = gethostbyname(hostname.c_str());
	if (host == 0) {
		cerr << "Yow! Bad host lookup." << endl;
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
	SerializedExchange response = Exchange::HandleMessage(m_Ai, request, m_ContinueToListen);
	send(m_CSocket, response.buffer, response.bufferSize, 0);
}