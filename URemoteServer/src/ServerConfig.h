#pragma once

#include <string>
#include "Properties.h"

class ServerConfig
{
public:
	ServerConfig(string configFile);
	~ServerConfig();
	
	int Port;
	int MaxConcurrentConnections;

private:
	int GetPortFromProperty(Properties prop);
	int GetMaxConnectionsFromProperty(Properties prop);
};