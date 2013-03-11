#include "AI.h"

#include <thread>

#include "modules\Speech.h"
#include "Translator.h"
#include "TextKey.h"
#include "Utils.h"
#include "exception\Exception.h"
#include "listeners\ConsoleListener.h"
#include "listeners\URemoteListener.h"
#include "listeners\VoiceListener.h"

#define DELAY 60*5 // 5 min / 300 sec before repeate time

//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////
AI::AI(std::unique_ptr<AIConfig> config) : m_config(move(config))
{
	try {
		m_voice = std::unique_ptr<Speech>(new Speech(m_config->Lang, m_config->Gender));

	} catch (const Exception& e) {
		Utils::getLogger()->error(e.whatAsString());
		// if an error occured, try with default language.
		// No exception catching this time.
		// TODO: On change lang => update Translator
		m_config->Lang = AIConfig::DEFAULT_LANG;
		
		m_voice = std::unique_ptr<Speech>(new Speech(m_config->Lang, m_config->Gender));
	}

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
	
	std::unique_ptr<Listener> uRemoteListener;
	std::unique_ptr<Listener> consoleListener;
	std::unique_ptr<Listener> voiceListener;
	//m_listeners2 = std::vector<std::unique_ptr<Listener>>();
	try {
		uRemoteListener = std::unique_ptr<URemoteListener>(new URemoteListener(move(serverConfig), this));
		uRemoteThread = uRemoteListener->start();
		Utils::getLogger()->debug("AI::StartConnection(), URemoteListener OK");
		m_listeners.push_back(std::move(uRemoteListener));

	} catch (const std::exception&) {
		Utils::getLogger()->error("AI::StartConnection(), URemoteListener KO");
	}
	
	try {
		voiceListener = std::unique_ptr<VoiceListener>(new VoiceListener(this));
		voiceRecoThread = voiceListener->start();
		Utils::getLogger()->debug("AI::StartConnection(), voiceRecoThread OK");
		m_listeners.push_back(std::move(voiceListener));

	} catch (const std::exception&) {
		Utils::getLogger()->error("AI::StartConnection(), voiceRecoThread KO");
	}

	// TODO: Replace console listener by UI Listener
	try {
		consoleListener = std::unique_ptr<ConsoleListener>(new ConsoleListener());
		consoleThread = consoleListener->start();
		Utils::getLogger()->debug("AI::StartConnection(), ConsoleListener OK");
		m_listeners.push_back(std::move(consoleListener));

	} catch (const std::exception&) {
		Utils::getLogger()->error("AI::StartConnection(), ConsoleListener KO");
	}

	// Notify the user that the listener are open.
	auto text = Translator::getString(TextKey::AI_SERVER_ONLINE, m_config->Name);
	say(text);

	// Join the listener threads
	// TODO: Make it automatic
	uRemoteThread.join();
	Utils::getLogger()->debug("AI::StartConnection(), uRemoteThread has joined");
	consoleThread.join();
	Utils::getLogger()->debug("AI::StartConnection(), consoleThread has joined");
	voiceRecoThread.join();
	Utils::getLogger()->debug("AI::StartConnection(), voiceRecoThread has joined");
}

void AI::stopConnection()
{
	for (const auto& listener : m_listeners) {
		listener->stop();
	}
}

std::string AI::getName()
{
	return m_config->Name;
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
	auto text = trad::get_string(TextKey::AI_SHUTDOWN, m_config->Name);
	say(text);
}