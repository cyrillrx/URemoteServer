#pragma once

#include <string> 
#include <WinSock2.h>

#include "server_exchange.pb.h"
#include "AI.h"

using namespace std;
using namespace network;

class Server
{

private :
	static int s_InstanceCount;

	int m_Port;
	int m_MaxConcurrentConnections;
	SOCKET m_CSocket;
	SOCKET m_ListenSocket;
	bool m_ContinueToListen;
	AI *m_ArtificialIntelligence;

	void InitServer();
	void FreeServer();
	void HandleMessage(string _msg);
	void Reply(string _message);
	string VolumeCommand(Request_Code _code);
	string AICommand(Request_Code _code);
	string ClassicCommand(Request_Code _code);
	string AppCommand(string _param);
	
	string ShutdownPC();
	
public:
	Server(int _port, int _maxConnections);
	~Server(void);

	void Launch();
};

