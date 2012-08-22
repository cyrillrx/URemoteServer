#include "Server.h"

#include <iostream>
#include <comdef.h>

#include "Exchange.h"

int Server::s_InstanceCount = 0;

//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////

Server::Server(int _port, int _maxConnections)
{
	m_Port = _port;
	m_MaxConcurrentConnections = _maxConnections;
	InitServer();
}

Server::~Server(void)
{
	FreeServer();
}

//! Lancement du serveur
bool Server::Start()
{
	m_ContinueToListen = true;

	// Socket d'acceptation
	//SOCKADDR_IN csin;
	//int sizeofcsin = sizeof(csin);
	char buffer[BUFSIZ];

	/* connection socket */
	
	while (m_ContinueToListen) {
		cout << "Server Info : " << endl;
		cout << " - Hostname   : " << m_Hostname << endl;
		cout << " - IP Address : " << m_IpAddress << endl;
		cout << " - Open port  : " << m_Port << endl;
		cout << "Waiting for client to connect..." << endl;
		
		//m_CSocket = accept(m_ListenSocket, (SOCKADDR *)&csin, &sizeofcsin);
		m_CSocket = accept(m_ListenSocket, NULL, NULL);
		if (m_CSocket == INVALID_SOCKET) {
			cerr << "accept failed with error: " << WSAGetLastError() << endl;
			FreeServer();
			return false;
		}
		
		memset(buffer, '\0', sizeof(buffer)); // On vide le buffer
		//___memset(buffer, '\0', BUFSIZ); // On vide le buffer
		int res = recv(m_CSocket, buffer, sizeof(buffer), 0);
		//___int res = recv(m_CSocket, buffer, BUFSIZ, 0);
		cout << "  -- result : " << res <<  endl << endl;
		void* data = (void*) buffer;
		//string message = buffer;
		HandleMessage(data);

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

//! Initialize the server
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
	socketAddress.sin_port			= htons(m_Port);

	// Bind the socket to the address and port defined in SOCKADDR
	if (bind(m_ListenSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress)) == SOCKET_ERROR) {
		cerr << "bind failed with error: " << WSAGetLastError() << endl;
		FreeServer();
		return false;
	}

	// Listen to incoming connections
	if (listen(m_ListenSocket, m_MaxConcurrentConnections) == SOCKET_ERROR) {
		cerr << "listen failed with error: " << WSAGetLastError() << endl;
		FreeServer();
		return false;
	}

	m_Hostname = GetHostName();
	m_IpAddress = GetIpAddress(m_Hostname);

	return true;
}

//! Libère les sockets
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

string Server::GetIpAddress(string _hostname)
{
	string ipAddress = "";
	struct hostent *host = gethostbyname(_hostname.c_str());
	if (host == 0) {
		cerr << "Yow! Bad host lookup." << endl;
	}

	/*
	for (int i = 0; host->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, host->h_addr_list[i], sizeof(struct in_addr));
		cout << "Address " << i << ": " << inet_ntoa(addr) << endl;
	}
	*/
	struct in_addr addr;
	memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
	ipAddress = inet_ntoa(addr);

	return ipAddress;
}

//! Traitement de la commande envoyée par le client
void Server::HandleMessage(void* _data) 
{
	string serializedReply = Exchange::HandleMessage(_data, m_ContinueToListen);
	Reply(serializedReply);
}

//! Envoie de la réponse au client
void Server::Reply(string _message)
{
	if (_message.empty()) {
		cout << "Server::Reply : Message is empty." << endl;
		return;
	}

	send(m_CSocket, _message.data(), strlen(_message.data()), 0);
	cout << _message << endl;
}