#pragma once

#include <string> 
#include <memory>
#include <basetsd.h>

#include "Listener.h"
#include "..\ServerConfig.h"
#include "..\server_exchange.pb.h"
#include "logger.h"

class SerializedExchange;
class AI;

class URemoteListener : public Listener
{	
public:
	URemoteListener(std::unique_ptr<ServerConfig> config, AI *ai);
	~URemoteListener();

private :
	static int s_instanceCount;

	AI * m_ai;
	logger* m_log;
	std::unique_ptr<ServerConfig> m_config;
	
	std::string m_hostname;
	std::string m_ipAddress;
	UINT_PTR m_cSocket;
	UINT_PTR m_listenSocket;
	
	void doStart();
	bool initServer();
	void freeServer();
	std::string getHostName();
	std::string getIpAddress(std::string hostname);

	void handleMessage(SerializedExchange serializedRequest);
};