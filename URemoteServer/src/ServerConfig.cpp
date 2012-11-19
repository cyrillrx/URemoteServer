#include "ServerConfig.h"


ServerConfig::ServerConfig(string configFile)
{
	Properties properties;
	properties.LoadProperties(configFile);
	
	Port = GetPortFromProperty(properties);
	MaxConcurrentConnections = GetMaxConnectionsFromProperty(properties);
}

ServerConfig::~ServerConfig()
{
}

int ServerConfig::GetPortFromProperty(Properties prop)
{
	try {
		return prop.GetInt("port");
	} catch (ReadPropertyException const& exception) {
		return 8082;
	}
}

int ServerConfig::GetMaxConnectionsFromProperty(Properties prop)
{
	try {
		return prop.GetInt("maxConnections");
	} catch (ReadPropertyException const& exception) {
		return 3;
	}
}