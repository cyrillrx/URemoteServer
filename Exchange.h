#ifndef EXCHANGE_INCLUDED
#define EXCHANGE_INCLUDED

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

	static string HandleMessage(void* _msg, bool &_continueToListen);
	//static string GetRequestType(Request_Type _type);
	//static string GetRequestCode(Request_Code _code);

private :
	static void ClassicCommand(Response* _reply, Request_Code _code);
	static void VolumeCommand(Response* _reply, Request_Code _code);
	static void AICommand(Response* _reply, Request_Code _code);
	static void AppCommand(Response* _reply, Request_Code _code);
	static void ShutdownPC(Response* _reply, int _delay);
};


#endif  // EXCHANGE_INCLUDED