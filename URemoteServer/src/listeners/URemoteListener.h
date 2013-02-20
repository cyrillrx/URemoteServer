#pragma once

#include <WinSock2.h>
#include <string> 
#include <memory>

#include "Listener.h"
#include "..\ServerConfig.h"
#include "..\server_exchange.pb.h"
#include "Logger.h"

class SerializedExchange;
class AI;

class URemoteListener : public Listener
{	
public:
	URemoteListener(std::unique_ptr<ServerConfig> config, AI *ai);
	~URemoteListener();

private :
	static int s_instanceCount;

	AI * m_Ai;
	Logger* m_log;
	std::unique_ptr<ServerConfig> m_config;
	
	std::string m_hostname;
	std::string m_ipAddress;
	SOCKET m_cSocket;
	SOCKET m_listenSocket;
	
	void doStart();
	bool InitServer();
	void FreeServer();
	std::string GetHostName();
	std::string GetIpAddress(std::string hostname);

	void HandleMessage(SerializedExchange serializedRequest);
};