#pragma once

#include <string> 
#include <WinSock2.h>
#include <memory>
#include "ServerConfig.h"
#include "server_exchange.pb.h"

class SerializedExchange;
class AI;

class Server
{	
public:
	Server(std::unique_ptr<ServerConfig> config, AI *ai);
	~Server(void);

	
	bool Start();
	bool Stop();

private :
	static int s_InstanceCount;

	AI * m_Ai;
	std::unique_ptr<ServerConfig> m_Config;
	
	std::string m_Hostname;
	std::string m_IpAddress;
	SOCKET m_CSocket;
	SOCKET m_ListenSocket;
	bool m_ContinueToListen;

	bool InitServer();
	void FreeServer();
	std::string GetHostName();
	std::string GetIpAddress(std::string hostname);

	void HandleMessage(SerializedExchange serializedRequest);
};