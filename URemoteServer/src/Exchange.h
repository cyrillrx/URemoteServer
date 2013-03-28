#pragma once

#include <string>
#include "network_io.h"
#include "network_io/server_exchange.pb.h"
#include "AI.h"

class AI;

class Exchange
{
public :
	static network_io::serialized_message handleMessage(AI* ai, network_io::serialized_message serializedRequest);

private :
	static void classicCommand(AI* ai, network_io::Response reply, network_io::Request_Code code);
	static void volumeCommand(network_io::Response reply, network_io::Request_Code code, int intParam);
	static void aICommand(AI* ai, network_io::Response reply, network_io::Request_Code code);
	static void appCommand(network_io::Response reply, network_io::Request_Code code);
	static void shutdownPC(AI* ai, network_io::Response reply, int delay);
};