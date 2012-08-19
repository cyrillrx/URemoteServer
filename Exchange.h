#pragma once

#include <string> 
#include "server_exchange.pb.h"

using namespace std;
using namespace network;

class Exchange
{

public :
	
	static const string HELLO_CLIENT;
	// Gestion des fenêtres et applications
	static const string APP_GOM_PLAYER;
	static const string KILL_GOM_PLAYER;
	static const string GOM_PLAYER_STRETCH;

	static string HandleMessage(string _msg, bool &_continueToListen);
	static string GetRequestType(Request_Type _type);
	static string GetRequestCode(Request_Code _code);

private :
	bool m_ContinueToListen;
	static Response* ClassicCommand(Request_Code _code);
	static Response* VolumeCommand(Request_Code _code);
	static Response* AICommand(Request_Code _code);
	static Response* AppCommand(string _param);
	static Response* ShutdownPC();
};

