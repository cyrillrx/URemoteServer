#include "URemoteListener.h"

#include "core/platform_config.h"
# if defined(WINDOWS_PLATFORM)
#   include <winsock2.h>
# endif

#include <iostream>
#include "network_io/tcp_server.h"
#include "exception/Exception.h"

#define BUFFER_SIZE 4096

URemoteListener::URemoteListener(std::unique_ptr<network_io::server_config> config, std::shared_ptr<AI> ai)
	: Listener("URemoteListener.log"), config_(move(config)), executor_(ai)
{
	log_.Info("******************************************************");
	log_.Info("*****                URemoteListener             *****");
	log_.Info("******************************************************");

	// Init hostname
	try {
		hostname_	= network_io::hostname();
	} catch (const Exception& e) {
		hostname_ = "localhost";
		log_.Error("URemoteListener::InitServer(), " + e.whatAsString());
	}

	// Init ip address
	try {
		ipAddress_	= network_io::ip_address(hostname_);
	} catch (const Exception& e) {
		ipAddress_ = "127.0.0.1";
		log_.Error("URemoteListener::InitServer(), " + e.whatAsString());
	}
}

URemoteListener::~URemoteListener()
{
	log_.Info("******************************************************");
	log_.Info("*****               ~URemoteListener             *****");
	log_.Info("******************************************************");
}

/**
 * Launch the server
 * @return true if everything went correctly. False otherwise
 */
void URemoteListener::doStart()
{
	log_.Info("Do start URemoteListener");
	log_.Debug("Server Info : ");
	log_.Debug(" - Hostname   : " + hostname_);
	log_.Debug(" - IP Address : " + ipAddress_);
	log_.Debug(" - Open port  : " + std::to_string(config_->port()));
	log_.Debug("Waiting for client to connect...");

	try {
		network_io::tcp_server server(config_->port(), config_->pool_size(), executor_);
		// Run the server until stopped.
		server.run();

	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
