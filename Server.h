#pragma once

#include <string> 
#include <WinSock2.h>

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

	void InitServer();
	void FreeServer();
	void TreatCommand(string _cmd);
	void Reply(string _message);
	
public:
	Server(int _port, int _maxConnections);
	~Server(void);

	void Launch();
};

