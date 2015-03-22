#include "URemoteListener.h"

#include "platform_config.h"
# if defined(WINDOWS_PLATFORM)
#   include <winsock2.h>
# endif

#include <iostream>
#include "string_utils.h"
#include "network_io.h"
#include "network_io/tcp_server.h"
#include "exception/Exception.h"

//#define BUFFER_SIZE BUFSIZ
#define BUFFER_SIZE 4096

URemoteListener::URemoteListener(std::unique_ptr<network_io::server_config> config, std::shared_ptr<AI> ai)
	: config_(move(config)), executor_(ai)
{
	log_ = logger("URemoteListener.log");
	log_.set_log_severity_console(logger::SEVERITY_LVL_WARNING);

	log_.info("******************************************************");
	log_.info("*****                URemoteListener             *****");
	log_.info("******************************************************");

	// Init hostname
	try {
		hostname_	= network_io::hostname();
	} catch (const Exception& e) {
		hostname_ = "localhost";
		log_.error("URemoteListener::InitServer(), " + e.whatAsString());
	}

	// Init ip address
	try {
		ipAddress_	= network_io::ip_address(hostname_);
	} catch (const Exception& e) {
		ipAddress_ = "127.0.0.1";
		log_.error("URemoteListener::InitServer(), " + e.whatAsString());
	}

}

URemoteListener::~URemoteListener()
{
	log_.info("******************************************************");
	log_.info("*****               ~URemoteListener             *****");
	log_.info("******************************************************");
}

/**
* Launch the server
* @return true if everything went correctly. False otherwise
*/
void URemoteListener::doStart()
{
	log_.info("Do start URemoteListener");
	log_.debug("Server Info : ");
	log_.debug(" - Hostname   : " + hostname_);
	log_.debug(" - IP Address : " + ipAddress_);
	log_.debug(" - Open port  : " + std::to_string(config_->port()));
	log_.debug("Waiting for client to connect...");

	try {
		network_io::tcp_server server(config_->port(), config_->pool_size(), executor_);
		// Run the server until stopped.
		server.run();

	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

