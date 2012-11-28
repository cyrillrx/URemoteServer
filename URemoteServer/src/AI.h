#pragma once

#include <string>
#include <time.h>
#include <memory>
#include "AIConfig.h"
#include "ServerConfig.h"
#include "Server.h"

class Server;

class AI
{
public:
	AI(std::unique_ptr<AIConfig> aiConfig);
	~AI();

	bool StartConnection(std::unique_ptr<ServerConfig> serverConfig);
	bool StopConnection();

	void Welcome();
	void Say(std::string textToSpeak);
	bool ToggleMute();

private:
	std::unique_ptr<AIConfig> m_Config;
	std::unique_ptr<Server> m_ExchangeServer;
	time_t m_LastWelcome;
	
	void Start();
	void Shutdown();
};