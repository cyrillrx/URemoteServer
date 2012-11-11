#pragma once

#include <string>
#include "server_exchange.pb.h"

using namespace std;
using namespace network;

class SerializedExchange 
{
public :
	int bufferSize;
	char* buffer;
};

class Exchange
{
public :
	
	static SerializedExchange HandleMessage(SerializedExchange _serializedExchange, bool &_continueToListen);
	static Request* GetRequest(SerializedExchange _request);
	static SerializedExchange GetSerializeResponse(Response* _response);

private :
	static void ClassicCommand(Response* _reply, Request_Code _code);
	static void VolumeCommand(Response* _reply, Request_Code _code, int _intParam);
	static void AICommand(Response* _reply, Request_Code _code);
	static void AppCommand(Response* _reply, Request_Code _code);
	static void ShutdownPC(Response* _reply, int _delay);
};