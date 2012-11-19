#pragma once

#include <string> 
#include <WinSock2.h>
#include <memory>
#include "ServerConfig.h"
#include "server_exchange.pb.h"

using namespace std;
using namespace network;

class SerializedExchange;
class AI;
class Server
{	
public:
	Server(unique_ptr<ServerConfig> config, AI *ai);
	~Server(void);
	
	bool Start();
	bool Stop();

private :
	static int s_InstanceCount;
	AI * m_Ai;
	unique_ptr<ServerConfig> m_Config;
	
	string m_Hostname;
	string m_IpAddress;
	SOCKET m_CSocket;
	SOCKET m_ListenSocket;
	bool m_ContinueToListen;

	bool InitServer();
	void FreeServer();
	string GetHostName();
	string GetIpAddress(string hostname);

	void HandleMessage(SerializedExchange request);
};