#pragma once

#include <string> 
#include <WinSock2.h>

#include "server_exchange.pb.h"

using namespace std;
using namespace network;

class Server
{	
public:
	Server(int _port, int _maxConnections);
	~Server(void);
	
	bool Start();
	bool Stop();

private :
	static int s_InstanceCount;
	
	string m_Hostname;
	string m_IpAddress;
	int m_Port;
	int m_MaxConcurrentConnections;
	SOCKET m_CSocket;
	SOCKET m_ListenSocket;
	bool m_ContinueToListen;

	bool InitServer();
	void FreeServer();
	string GetHostName();
	string GetIpAddress(string _hostname);

	void HandleMessage(void* _data);
};