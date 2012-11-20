#pragma once

#include <string>
#include "Properties.h"

class ServerConfig : public Properties
{
public:
	ServerConfig(const string& path);
	~ServerConfig();
	
	int Port;
	int MaxConcurrentConnections;

private:
	static const string KEY_PORT;
	static const string KEY_MAX_CONNECTIONS;
	
	static const int DEFAULT_PORT;
	static const int DEFAULT_MAX_CONNECTIONS;
};