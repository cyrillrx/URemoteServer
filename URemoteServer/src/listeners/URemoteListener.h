#pragma once

#include <string>
#include <memory>

#include "Listener.h"
#include "logger.h"
#include "network_io/server_config.h"
#include "../executor.h"
#include "../AI.h"

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