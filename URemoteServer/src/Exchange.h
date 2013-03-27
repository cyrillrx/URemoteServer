#pragma once

#include <string>
#include "network_io/server_exchange.pb.h"
#include "AI.h"

class AI;

class SerializedExchange 
{
public :
	int bufferSize;
	char* buffer;
};

class Exchange
{
public :
	static SerializedExchange handleMessage(AI* ai, SerializedExchange serializedRequest);
	static network_io::Request* getRequest(SerializedExchange serializedRequest);
	static SerializedExchange getSerializeResponse(network_io::Response* response);

private :
	static void classicCommand(AI* ai, network_io::Response* reply, network_io::Request_Code code);
	static void volumeCommand(network_io::Response* reply, network_io::Request_Code code, int intParam);
	static void aICommand(AI* ai, network_io::Response* reply, network_io::Request_Code code);
	static void appCommand(network_io::Response* reply, network_io::Request_Code code);
	static void shutdownPC(AI* ai, network_io::Response* reply, int delay);
};