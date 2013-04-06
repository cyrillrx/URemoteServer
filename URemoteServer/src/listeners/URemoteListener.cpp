#include "URemoteListener.h"

# if defined(WINDOWS_PLATFORM)
#   include <WinSock2.h>
//  Link with ws2_32.lib
#   pragma comment(lib, "ws2_32.lib")
# endif

#include <iostream>
#include "string_utils.h"
#include "network_io.h"
#include "network_io/server_config.h"
#include "network_io/tcp_server.h"
#include "exception/Exception.h"
#include "../Exchange.h"

//#define BUFFER_SIZE BUFSIZ
#define BUFFER_SIZE 4096

using namespace network_io;

int URemoteListener::s_instanceCount = 0;

//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////

URemoteListener::URemoteListener(std::unique_ptr<server_config> config, AI* ai)
	: config_(move(config)), ai_(ai)
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

	freeServer();
}

/**
* Launch the server
* @return true if everything went correctly. False otherwise
*/
void URemoteListener::doStart()
{
	log_.info("Do start");
/*
	try {
		// Initialise the server.
		network_io::request_handler handler = std::bind(&URemoteListener::handleMessage, this);
		network_io::tcp_server server(config_->port(), config_->port(), handler);
		// Run the server until stopped.
		server.run();

	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	*/
	initServer();

	continueToListen_ = true;
	char buffer[BUFFER_SIZE];

	while (continueToListen_) {
		log_.debug("Server Info : ");
		log_.debug(" - Hostname   : " + hostname_);
		log_.debug(" - IP Address : " + ipAddress_);
		log_.debug(" - Open port  : " + std::to_string(config_->port()));
		log_.debug("Waiting for client to connect...");

		cSocket_ = ::accept(listenSocket_, nullptr, nullptr);
		if (cSocket_ == INVALID_SOCKET) {
			log_.error("accept() failed with error: " + std::to_string(WSAGetLastError()));
			freeServer();
			return;
		}

		string_utils::clear_buffer(buffer);

		int received = recv(cSocket_, buffer, sizeof(buffer), 0);
		log_.debug("  -- result : " + received);

		serialized_message request(buffer, received);

		auto response = handleMessage(request);
		send(cSocket_, response.buffer(), response.size(), 0);

		closesocket(cSocket_);
		log_.debug("Socket closed.");
	}
}

//////////////////////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////////////////////

/**
* Initialize the server.
*/
bool URemoteListener::initServer()
{
	// Initialize winSock library (v2.0)
	log_.debug("Initializing winSock library (v2.0)...");
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,0), &wsaData) != NO_ERROR) {
		log_.error("URemoteListener::InitServer(), WSAStartup() failed with error: " + std::to_string(WSAGetLastError()));
		WSACleanup();
		return false;
	}

	s_instanceCount++;

	// Create listener socket for incoming connections
	log_.debug("Creating listener socket for incoming connections...");
	listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (listenSocket_ == INVALID_SOCKET) {
		log_.error("URemoteListener::InitServer(), socket() failed with error: " + std::to_string(WSAGetLastError()));
		freeServer();
		return false;
	}

	// Socket thecnical info
	SOCKADDR_IN socketAddress;
	socketAddress.sin_addr.s_addr	= htonl(INADDR_ANY); // Server address
	socketAddress.sin_family		= AF_INET; // Type of socket (AF_INET = Internet)
	socketAddress.sin_port			= htons(config_->port());

	// Bind the socket to the address and port defined in SOCKADDR
	log_.debug("Binding the socket to the address and port...");
	if (::bind(listenSocket_, (SOCKADDR*)&socketAddress, sizeof(socketAddress)) == SOCKET_ERROR) {
		log_.error("URemoteListener::InitServer(), bind() failed with error: " + std::to_string(WSAGetLastError()));
		freeServer();
		return false;
	}

	// Listen to incoming connections
	log_.debug("Listen to incoming connections...");
	if (::listen(listenSocket_, config_->pool_size()) == SOCKET_ERROR) {
		log_.error("URemoteListener::InitServer(), listen() failed with error: " + std::to_string(WSAGetLastError()));
		freeServer();
		return false;
	}

	return true;
}

/** Free the sockets. */
void URemoteListener::freeServer()
{
	closesocket(listenSocket_);

	s_instanceCount--;
	if (s_instanceCount < 1) {
		WSACleanup();
	}
}

/**
* Handle the command sent by the client.
* then send a response.
*/
serialized_message URemoteListener::handleMessage(serialized_message request)
{
	return Exchange::handleMessage(ai_, request);
}

//TODO: Define a function (inline) for std::to_string(WSAGetLastError())
