#include "ServerConfig.h"

using namespace std;

const string ServerConfig::KEY_PORT = "port";
const string ServerConfig::KEY_MAX_CONNECTIONS = "maxConnections";

const int ServerConfig::DEFAULT_PORT = 8082;
const int ServerConfig::DEFAULT_MAX_CONNECTIONS = 3;

ServerConfig::ServerConfig(const string& configFile) : Properties(configFile)
{
	Port = getInt(KEY_PORT, DEFAULT_PORT);
	MaxConcurrentConnections = getInt(KEY_MAX_CONNECTIONS, DEFAULT_MAX_CONNECTIONS);
}

ServerConfig::~ServerConfig()
{
	setInt(KEY_PORT, Port);
	setInt(KEY_MAX_CONNECTIONS, MaxConcurrentConnections);
	saveProperties();
}
