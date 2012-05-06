#pragma once

#include <string> 
#include <WinSock2.h>

#include "AI.h"

using namespace std;

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
	void TreatCommand(string _cmd);
	void Reply(string _message);
	string VolumeCommand(string _param);
	string AICommand(string _param);
	string ClassicCommand(string _param);
	string AppCommand(string _param);
	
public:
	Server(int _port, int _maxConnections);
	~Server(void);

	void Launch();
};

