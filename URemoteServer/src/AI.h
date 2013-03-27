#pragma once

#include <string>
#include <ctime>
#include <memory>
#include <vector>

#include "AIConfig.h"
#include "network_io/server_config.h"
#include "listeners/Listener.h"

class Speech;
class Server;

class AI
{
public:
	AI(std::unique_ptr<AIConfig> aiConfig);
	~AI();

	void startConnection(std::unique_ptr<network_io::server_config> serverConfig);
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
