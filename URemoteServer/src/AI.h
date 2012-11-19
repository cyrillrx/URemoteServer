#pragma once

#include <string>
#include <time.h>
#include <memory>
#include "AIConfig.h"
#include "ServerConfig.h"
#include "Server.h"

using namespace std;
class Server;
class AI
{
public:
	AI(unique_ptr<AIConfig> aiConfig);
	~AI();

	bool StartConnection(unique_ptr<ServerConfig> serverConfig);
	bool StopConnection();

	void Welcome();
	void Say(string textToSpeak);
	bool ToggleMute();

private:
	unique_ptr<AIConfig> m_Config;
	unique_ptr<Server> m_ExchangeServer;
	time_t m_LastWelcome;
	
	void Start();
	void Shutdown();
};