#pragma once

#include <string>
#include <memory>

#include "Listener.h"
#include "logger.h"
#include "../executor.h"
#include "../AI.h"

// TODO update Biicode config
//#include "network_io/server_config.h"
#include "cyrillrx/cross_api/src/network_io/server_config.h"

class URemoteListener : public Listener
{
public:
	URemoteListener(std::unique_ptr<network_io::server_config> config, std::shared_ptr<AI> ai);
	virtual ~URemoteListener();

private :
	virtual void doStart();

	executor executor_;
	std::unique_ptr<network_io::server_config> config_;

	std::string hostname_;
	std::string ipAddress_;
};