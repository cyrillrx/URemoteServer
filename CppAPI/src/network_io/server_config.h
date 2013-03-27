#pragma once

#include <string>
#include "Properties.h"

namespace network_io
{
	class server_config : public Properties
	{
	public:
		server_config(const std::string& path);
		~server_config();

		unsigned short port();
		int max_concurrent_connections();

	private:
		static const std::string KEY_PORT;
		static const std::string KEY_MAX_CONNECTIONS;

		static const int DEFAULT_PORT;
		static const int DEFAULT_MAX_CONNECTIONS;

		unsigned short m_port;
		int m_max_concurrent_connections;
	};
}