#include "AI.h"

#include <thread>

#include "modules/Speech.h"
#include "Translator.h"
#include "trad_key.h"
#include "Utils.h"
#include "exception/Exception.h"
#include "listeners/ConsoleListener.h"
#include "listeners/URemoteListener.h"
#include "listeners/VoiceListener.h"

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

	std::unique_ptr<Listener> uRemoteListener;
	std::unique_ptr<Listener> consoleListener;
	std::unique_ptr<Listener> voiceListener;
	//m_listeners2 = std::vector<std::unique_ptr<Listener>>();

	int capacity = 0;
	try {
		uRemoteListener = std::unique_ptr<URemoteListener>(new URemoteListener(move(serverConfig), this));
		uRemoteThread = uRemoteListener->start();
		Utils::getLogger()->debug("AI::StartConnection(), URemoteListener OK");
		m_listeners.push_back(std::move(uRemoteListener));
		capacity += 40;
		say("URemote Listener, OK..."); // TODO: internationalize

	} catch (const std::exception&) {
		Utils::getLogger()->error("AI::StartConnection(), URemoteListener KO");
		say("URemote Listener, KO..."); // TODO: internationalize
	}

	try {
		voiceListener = std::unique_ptr<VoiceListener>(new VoiceListener(this));
		voiceRecoThread = voiceListener->start();
		Utils::getLogger()->debug("AI::StartConnection(), VoiceListener OK");
		m_listeners.push_back(std::move(voiceListener));
		capacity += 30;
		say("Voice Listener, OK..."); // TODO: internationalize

	} catch (const std::exception&) {
		Utils::getLogger()->error("AI::StartConnection(), VoiceListener KO");
		say("Voice Listener, KO..."); // TODO: internationalize
	}

	// TODO: Replace console listener by UI Listener
	try {
		consoleListener = std::unique_ptr<ConsoleListener>(new ConsoleListener());
		consoleThread = consoleListener->start();
		Utils::getLogger()->debug("AI::StartConnection(), ConsoleListener OK");
		m_listeners.push_back(std::move(consoleListener));
		capacity += 30;
		say("Console Listener, OK..."); // TODO: internationalize

	} catch (const std::exception&) {
		Utils::getLogger()->error("AI::StartConnection(), ConsoleListener KO");
		say("Console Listener, KO..."); // TODO: internationalize
	}

	// Notify the user that the listener are open.
	if (capacity >= 100) {
		say(Translator::getString(trad_key::AI_FULL_CAPACITY));
	} else {
		say(trad::get_string(trad_key::AI_NOT_FULL_CAPACITY, capacity));
	}

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
		// TODO: Welcome user instead of welcoming itself
		say(trad::get_string(trad_key::AI_WELCOME_USER, m_config->Name));
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
	say(Translator::getString(trad_key::AI_INITIATED));
	say(trad::get_string(trad_key::AI_SELF_INTRODUCTION, m_config->Name));
}

void AI::shutdown()
{
	auto text = trad::get_string(trad_key::AI_SHUTDOWN, m_config->Name);
	say(text);
}
