#pragma once

#include <string>
#include <time.h>
#include "Server.h"

using namespace std;

class AI
{
private:
	static AI* s_Instance; 
	string m_Name;
	bool m_IsMute;
	time_t m_LastWelcome;
	Server *m_ExchangeServer;
	
	AI(string _name);
	~AI(void);
	void Start();
	void Shutdown();

public:
	static AI* GetInstance();
	static void FreeInstance();

	bool StartConnection(int _port, int _maxConnections);
	bool StopConnection();

	void Welcome();
	void Say(string _textToSpeak);
	bool ToggleMute();
};

