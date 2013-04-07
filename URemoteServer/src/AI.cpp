#include "AI.h"

#include <thread>

#include "modules/Speech.h"
#include "lexicon_manager.h"
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
AI::AI(std::unique_ptr<ai_config> config) : config_(move(config))
{
	voice_ = std::unique_ptr<Speech>(new Speech(config_->language, config_->gender));

	time(&lastWelcome_);
	lastWelcome_ -= DELAY;

	start();
}

AI::~AI()
{
	shutdown();
}

void AI::startConnection(std::unique_ptr<network_io::server_config> serverConfig)
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

	auto ai_ptr = shared_from_this();
	int capacity = 0;
	try {
		uRemoteListener = std::unique_ptr<URemoteListener>(new URemoteListener(move(serverConfig), ai_ptr));
		uRemoteThread = uRemoteListener->start();
		Utils::get_logger()->debug("AI::StartConnection(), URemoteListener OK");
		listeners_.push_back(std::move(uRemoteListener));
		capacity += 40;

	} catch (const std::exception& e) {
		Utils::get_logger()->error("AI::StartConnection(), URemoteListener KO : " + std::string(e.what()));
		say("URemote Listener, KO..."); // TODO: internationalize
	}

	try {
		voiceListener = std::unique_ptr<VoiceListener>(new VoiceListener(ai_ptr));
		voiceRecoThread = voiceListener->start();
		Utils::get_logger()->debug("AI::StartConnection(), VoiceListener OK");
		listeners_.push_back(std::move(voiceListener));
		capacity += 30;

	} catch (const std::exception& e) {
		Utils::get_logger()->error("AI::StartConnection(), VoiceListener KO : " + std::string(e.what()));
		say("Voice Listener, KO..."); // TODO: internationalize
	}

	// TODO: Replace console listener by UI Listener
	try {
		consoleListener = std::unique_ptr<ConsoleListener>(new ConsoleListener());
		consoleThread = consoleListener->start();
		Utils::get_logger()->debug("AI::StartConnection(), ConsoleListener OK");
		listeners_.push_back(std::move(consoleListener));
		capacity += 30;

	} catch (const std::exception& e) {
		Utils::get_logger()->error("AI::StartConnection(), ConsoleListener KO : " + std::string(e.what()));
		say("Console Listener, KO..."); // TODO: internationalize
	}

	// Notify the user that the listener are open.
	if (capacity >= 100) {
		say(lexicon_manager::get_string(trad_key::AI_FULL_CAPACITY));
	} else {
		say(lexicon_manager::get_string(trad_key::AI_NOT_FULL_CAPACITY, capacity));
	}

	// Join the listener threads
	// TODO: Make it automatic
	uRemoteThread.join();
	Utils::get_logger()->debug("AI::StartConnection(), uRemoteThread has joined");
	consoleThread.join();
	Utils::get_logger()->debug("AI::StartConnection(), consoleThread has joined");
	voiceRecoThread.join();
	Utils::get_logger()->debug("AI::StartConnection(), voiceRecoThread has joined");
}

void AI::stopConnection()
{
	for (const auto& listener : listeners_) {
		listener->stop();
	}
}

std::string AI::getName()
{
	return config_->name;
}

void AI::welcome()
{
	// Calculate the elapsed time since the last call to the method
	time_t now;
	time(&now);
	Utils::get_logger()->debug("AI::Welcome, now " + std::to_string(now));
	auto elapsedTime = difftime(now, lastWelcome_);
	Utils::get_logger()->debug("AI::Welcome, elapsedTime " + std::to_string(elapsedTime));

	// Welcome if last welcome > DELAY
	if (elapsedTime > DELAY) {
		// TODO: Welcome user instead of welcoming itself
		say(lexicon_manager::get_string(trad_key::AI_WELCOME_USER, config_->name));
		time(&lastWelcome_);
	}
}

void AI::say(std::string textToSpeak)
{
	// Test mute state
	if (!config_->is_mute) {
		voice_->sayInThread(textToSpeak);
	}
}

//! Change l'état de mute et renvoie le nouvel état
bool AI::toggleMute()
{
	config_->is_mute = !config_->is_mute;
	return config_->is_mute;
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

void AI::start()
{
	say(lexicon_manager::get_string(trad_key::AI_INITIATED));
	say(lexicon_manager::get_string(trad_key::AI_SELF_INTRODUCTION, config_->name));
}

void AI::shutdown()
{
	auto text = lexicon_manager::get_string(trad_key::AI_SHUTDOWN, config_->name);
	say(text);
}
