#pragma once

#include <string>
#include "Properties.h"

class ServerConfig : public Properties
{
public:
	ServerConfig(const std::string& path);
	~ServerConfig();
	
	int Port;
	int MaxConcurrentConnections;

private:
	static const std::string KEY_PORT;
	static const std::string KEY_MAX_CONNECTIONS;
	
	static const int DEFAULT_PORT;
	static const int DEFAULT_MAX_CONNECTIONS;
};