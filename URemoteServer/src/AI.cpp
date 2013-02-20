#include "AI.h"

#include <thread>

#include "modules\Speech.h"
#include "Translator.h"
#include "TextKey.h"
#include "Utils.h"

#define DELAY 60*5 // 5 min / 300 sec before repeate time

//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////
AI::AI(std::unique_ptr<AIConfig> config) : m_config(move(config))
{
	m_voice = std::unique_ptr<Speech>(new Speech(m_config->Lang, m_config->Gender));
	time(&m_lastWelcome);
	m_lastWelcome -= DELAY;

	start();
}

AI::~AI()
{
	shutdown();
}

void AI::startConnection(std::unique_ptr<ServerConfig> serverConfig)
{
	// TODO: Instanciate other listeners
	std::thread uRemoteThread;
	std::thread consoleThread;
	std::thread voiceRecoThread;
	//thread uiListener;

	// TODO: Change debug messages
	try {
		m_uRemoteListener = std::unique_ptr<URemoteListener>(new URemoteListener(move(serverConfig), this));
		uRemoteThread = m_uRemoteListener->start();
		Utils::getLogger()->debug("AI::StartConnection(), URemoteListener OK");

	} catch (const std::exception&) {
		Utils::getLogger()->error("AI::StartConnection(), URemoteListener KO");
	}
	
	try {
		m_voiceListener = std::unique_ptr<VoiceListener>(new VoiceListener());
		voiceRecoThread = m_voiceListener->start();
		Utils::getLogger()->debug("AI::StartConnection(), voiceRecoThread OK");

	} catch (const std::exception&) {
		Utils::getLogger()->error("AI::StartConnection(), voiceRecoThread KO");
	}

	// TODO: Replace console listener by UI Listener
	try {
		m_consoleListener = std::unique_ptr<ConsoleListener>(new ConsoleListener());
		consoleThread = m_consoleListener->start();
		Utils::getLogger()->debug("AI::StartConnection(), ConsoleListener OK");

	} catch (const std::exception&) {
		Utils::getLogger()->error("AI::StartConnection(), ConsoleListener KO");
	}

	// Notify the user that the listener are open.
	auto text = Translator::getString(TextKey::AI_SERVER_ONLINE, m_config->Name);
	say(text);

	// Join the listener threads
	// TODO: Make it automatic
	uRemoteThread.join();
	consoleThread.join();
	voiceRecoThread.join();
}

void AI::stopConnection()
{
	// TODO: Make it automatic
	if (m_uRemoteListener) {
		m_uRemoteListener->Stop();
	}

	if (m_consoleListener) {
		m_consoleListener->Stop();
	}
	
	if (m_voiceListener) {
		m_voiceListener->Stop();
	}
}

void AI::welcome()
{
	// Calculate the elapsed time since the last call to the method
	time_t now;
	time(&now);
	Utils::getLogger()->debug("AI::Welcome, now " + std::to_string(now));
	auto elapsedTime = difftime(now, m_lastWelcome);
	Utils::getLogger()->debug("AI::Welcome, elapsedTime " + std::to_string(elapsedTime));

	// Welcome if last welcome > DELAY
	if (elapsedTime > DELAY) {
		auto text = Translator::getString(TextKey::AI_WELCOME, m_config->Name);
		say(text);
		time(&m_lastWelcome);
	}
}

void AI::say(std::string textToSpeak)
{
	// Test mute state
	if (!m_config->IsMute) {
		m_voice->sayInThread(textToSpeak);
	}
}

//! Change l'état de mute et renvoie le nouvel état
bool AI::toggleMute()
{
	m_config->IsMute = !m_config->IsMute;
	return m_config->IsMute;
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

void AI::start()
{
	auto text = Translator::getString(TextKey::AI_INITIATED);
	say(text);
}

void AI::shutdown()
{
	// TODO: clean the function
	auto text = getString(TextKey::AI_SHUTDOWN, m_config->Name);
	//auto text = Translator::getString(TextKey::AI_SHUTDOWN, m_Config->Name);
	say(text);
}