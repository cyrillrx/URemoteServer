#pragma once

#include <string>
#include "server_exchange.pb.h"
#include "AI.h"

using namespace std;
using namespace network;

class SerializedExchange 
{
public :
	int bufferSize;
	char* buffer;
};

class AI;
class Exchange
{
public :
	
	static SerializedExchange HandleMessage(AI* ai, SerializedExchange serializedRequest, bool &continueToListen);
	static Request* GetRequest(SerializedExchange request);
	static SerializedExchange GetSerializeResponse(Response* response);

private :
	static void ClassicCommand(AI* ai, Response* reply, Request_Code code);
	static void VolumeCommand(Response* reply, Request_Code code, int intParam);
	static void AICommand(AI* ai, Response* reply, Request_Code code);
	static void AppCommand(Response* reply, Request_Code code);
	static void ShutdownPC(AI* ai, Response* reply, int delay);
};