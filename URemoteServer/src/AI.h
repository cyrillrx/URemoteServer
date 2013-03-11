#pragma once

#include <string>
#include <ctime>
#include <memory>
#include <vector>

#include "AIConfig.h"
#include "ServerConfig.h"
#include "listeners\Listener.h"

class Speech;
class Server;

class AI
{
public:
	AI(std::unique_ptr<AIConfig> aiConfig);
	~AI();

	void startConnection(std::unique_ptr<ServerConfig> serverConfig);
	void stopConnection();

	std::string getName();

	void welcome();
	void say(std::string textToSpeak);
	bool toggleMute();

private:
	std::unique_ptr<AIConfig> m_config;
	std::unique_ptr<Speech> m_voice;
	
	std::vector<std::unique_ptr<Listener>> m_listeners;
	time_t m_lastWelcome;
	
	void start();
	void shutdown();
};