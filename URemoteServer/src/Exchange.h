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
	static SerializedExchange HandleMessage(AI* ai, SerializedExchange serializedRequest, bool &continueToListen);
	static network::Request* GetRequest(SerializedExchange serializedRequest);
	static SerializedExchange GetSerializeResponse(network::Response* response);

private :
	static void ClassicCommand(AI* ai, network::Response* reply, network::Request_Code code);
	static void VolumeCommand(network::Response* reply, network::Request_Code code, int intParam);
	static void AICommand(AI* ai, network::Response* reply, network::Request_Code code);
	static void AppCommand(network::Response* reply, network::Request_Code code);
	static void ShutdownPC(AI* ai, network::Response* reply, int delay);
};