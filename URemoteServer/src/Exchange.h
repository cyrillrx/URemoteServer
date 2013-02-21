#pragma once

#include <string>
#include "server_exchange.pb.h"
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
	static SerializedExchange handleMessage(AI* ai, SerializedExchange serializedRequest, bool &continueToListen);
	static network::Request* getRequest(SerializedExchange serializedRequest);
	static SerializedExchange getSerializeResponse(network::Response* response);

private :
	static void classicCommand(AI* ai, network::Response* reply, network::Request_Code code);
	static void volumeCommand(network::Response* reply, network::Request_Code code, int intParam);
	static void aICommand(AI* ai, network::Response* reply, network::Request_Code code);
	static void appCommand(network::Response* reply, network::Request_Code code);
	static void shutdownPC(AI* ai, network::Response* reply, int delay);
};