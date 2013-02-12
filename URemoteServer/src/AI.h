#pragma once

#include <string>
#include <time.h>
#include <memory>
#include "AIConfig.h"
#include "ServerConfig.h"
#include "listeners\URemoteListener.h"
#include "listeners\ConsoleListener.h"

class Speech;
class Server;

class AI
{
public:
	AI(std::unique_ptr<AIConfig> aiConfig);
	~AI();

	void StartConnection(std::unique_ptr<ServerConfig> serverConfig);
	void StopConnection();

	void Welcome();
	void Say(std::string textToSpeak);
	bool ToggleMute();

private:
	std::unique_ptr<AIConfig> m_Config;
	std::unique_ptr<Speech> m_Voice;
	std::unique_ptr<URemoteListener> m_uRemoteListener;
	std::unique_ptr<ConsoleListener> m_consoleListener;
	time_t m_LastWelcome;
	
	void Start();
	void Shutdown();
};