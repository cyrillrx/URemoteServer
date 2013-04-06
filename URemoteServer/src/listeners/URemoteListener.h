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
#include "network_io/serialized_message.h"
#include "logger.h"

class AI;

class URemoteListener : public Listener
{
public:
	URemoteListener(std::unique_ptr<network_io::server_config> config, AI *ai);
	virtual ~URemoteListener();

private :
	virtual void doStart();
	static int s_instanceCount;

	AI * ai_;
	std::unique_ptr<network_io::server_config> config_;

	std::string hostname_;
	std::string ipAddress_;


#if defined(WINDOWS_PLATFORM)
	UINT_PTR cSocket_;
	UINT_PTR listenSocket_;
#endif

	bool initServer();
	void freeServer();
	
	//void handleMessage(network_io::serialized_message serializedRequest);
	network_io::serialized_message handleMessage(network_io::serialized_message serializedRequest);
};
