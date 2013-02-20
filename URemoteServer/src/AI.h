#pragma once

#include <string>
#include <time.h>
#include <memory>

#include "AIConfig.h"
#include "ServerConfig.h"
#include "listeners\URemoteListener.h"
#include "listeners\ConsoleListener.h"
#include "listeners\VoiceListener.h"

class Speech;
class Server;

class AI
{
public:
	AI(std::unique_ptr<AIConfig> aiConfig);
	~AI();

	void startConnection(std::unique_ptr<ServerConfig> serverConfig);
	void stopConnection();

	void welcome();
	void say(std::string textToSpeak);
	bool toggleMute();

private:
	std::unique_ptr<AIConfig> m_config;
	std::unique_ptr<Speech> m_voice;

	std::unique_ptr<URemoteListener> m_uRemoteListener;
	std::unique_ptr<ConsoleListener> m_consoleListener;
	std::unique_ptr<VoiceListener> m_voiceListener;
	time_t m_lastWelcome;
	
	void start();
	void shutdown();
};