#pragma once

#include "platform_config.h"

# if defined(WINDOWS_PLATFORM)
#   include <basetsd.h>
# endif

#include <string>
#include <memory>

#include "Listener.h"
#include "network_io.h"
#include "network_io/server_config.h"
#include "network_io/server_exchange.pb.h"
#include "logger.h"

//class SerializedExchange;
class AI;

class URemoteListener : public Listener
{
public:
	URemoteListener(std::unique_ptr<network_io::server_config> config, AI *ai);
	virtual ~URemoteListener();

private :
	virtual void doStart();
	static int s_instanceCount;

	AI * m_ai;
	std::unique_ptr<network_io::server_config> m_config;

	std::string m_hostname;
	std::string m_ipAddress;


#if defined(WINDOWS_PLATFORM)
	UINT_PTR m_cSocket;
	UINT_PTR m_listenSocket;
#endif

	bool initServer();
	void freeServer();

	void handleMessage(network_io::serialized_message serializedRequest);
};
