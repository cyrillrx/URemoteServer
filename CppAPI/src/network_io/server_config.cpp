#include "server_config.h"

namespace network_io
{
	const std::string server_config::KEY_PORT = "port";
	const std::string server_config::KEY_MAX_CONNECTIONS = "maxConnections";

	const int server_config::DEFAULT_PORT = 8082;
	const int server_config::DEFAULT_MAX_CONNECTIONS = 3;

	server_config::server_config(const std::string& configFile) : Properties(configFile)
	{
		m_port = getInt(KEY_PORT, DEFAULT_PORT);
		m_max_concurrent_connections = getInt(KEY_MAX_CONNECTIONS, DEFAULT_MAX_CONNECTIONS);
	}

	server_config::~server_config()
	{
		setInt(KEY_PORT, m_port);
		setInt(KEY_MAX_CONNECTIONS, m_max_concurrent_connections);
		saveProperties();
	}

	unsigned short server_config::port()
	{
		return m_port;
	}

	int server_config::max_concurrent_connections()
	{
		return m_max_concurrent_connections;
	}
}