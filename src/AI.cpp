#include "AI.h"

#include <thread>

#include "logger/console_logger.h"
#include "lang/lexicon_manager.h"
#include "ai_config.h"
#include "authorized_users.h"
#include "listeners/ConsoleListener.h"
#include "listeners/URemoteListener.h"
#include "listeners/VoiceListener.h"
#include "trad_key.h"

//! Delay for the AI to say welcome again
#define DELAY 60*5 // 5 min / 300 sec

auto log = ConsoleLogger(DEBUG);

//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////
AI::AI(std::unique_ptr<ai_config> config, std::unique_ptr<authorized_users> users)
        : config_(move(config)), users_(move(users))
{
    voice_ = std::unique_ptr<text_to_speech::voice>(
            new text_to_speech::voice(config_->name, config_->language_code(), config_->gender, config_->age,
                                      config_->rate));
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
    //say(lexicon_manager::get_string(trad_key::AI_SETTING_UP));
    log.Info(lexicon_manager::get_string(trad_key::AI_SETTING_UP));

    // TODO: Instantiate other listeners
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
        uRemoteListener = std::unique_ptr<Listener>(new URemoteListener(move(serverConfig), ai_ptr));
        uRemoteThread = uRemoteListener->start();
        log.Debug("AI::StartConnection(), URemoteListener OK");
        listeners_.push_back(std::move(uRemoteListener));
        capacity += 40;

    } catch (const std::exception& e) {
        log.Error("AI::StartConnection(), URemoteListener KO : " + std::string(e.what()));
        say("URemote Listener, KO..."); // TODO: internationalize
    }

    try {
        voiceListener = std::unique_ptr<Listener>(new VoiceListener(ai_ptr));
        voiceRecoThread = voiceListener->start();
        log.Debug("AI::StartConnection(), VoiceListener OK");
        listeners_.push_back(std::move(voiceListener));
        capacity += 30;

    } catch (const std::exception& e) {
        log.Error("AI::StartConnection(), VoiceListener KO : " + std::string(e.what()));
        say("Voice Listener, KO..."); // TODO: internationalize
    }

    // TODO: Replace console listener by UI Listener
    try {
        consoleListener = std::unique_ptr<Listener>(new ConsoleListener());
        consoleThread = consoleListener->start();
        log.Debug("AI::StartConnection(), ConsoleListener OK");
        listeners_.push_back(std::move(consoleListener));
        capacity += 30;

    } catch (const std::exception& e) {
        log.Error("AI::StartConnection(), ConsoleListener KO : " + std::string(e.what()));
        say("Console Listener, KO..."); // TODO: internationalize
    }

    //say(lexicon_manager::get_string(trad_key::AI_CONFIG_COMPLETED));
    log.Info(lexicon_manager::get_string(trad_key::AI_CONFIG_COMPLETED));

    // Notify the user that the listener are open.
    if (capacity >= 100) {
        say(lexicon_manager::get_string(trad_key::AI_FULL_CAPACITY));
    } else {
        say(lexicon_manager::get_string(trad_key::AI_NOT_FULL_CAPACITY, capacity));
    }

    // Join the listener threads
    // TODO: Make it automatic
    voiceRecoThread.join();
    log.Debug("AI::StartConnection(), voiceRecoThread has joined");
    uRemoteThread.join();
    log.Debug("AI::StartConnection(), uRemoteThread has joined");
    consoleThread.join();
    log.Debug("AI::StartConnection(), consoleThread has joined");
}

void AI::stopConnection()
{
    for (const auto& listener : listeners_) {
        listener->stop();
    }
}

bool AI::isAuthorized(const std::string& securityToken)
{
    return users_->is_authorized(securityToken);
}

std::string AI::getUser(const std::string& securityToken)
{
    return users_->get_user(securityToken);
}

std::string AI::getName()
{
    return config_->name;
}

void AI::welcome(const std::string& securityToken)
{
    // Calculate the elapsed time since the last call to the method
    time_t now;
    time(&now);
    log.Debug("AI::Welcome, now " + std::to_string(now));
    auto elapsedTime = difftime(now, lastWelcome_);
    log.Debug("AI::Welcome, elapsedTime " + std::to_string(elapsedTime));

    // Welcome if last welcome > DELAY
    if (elapsedTime > DELAY) {
        // TODO: Welcome user instead of welcoming itself
        say(lexicon_manager::get_string(trad_key::AI_WELCOME_USER, users_->get_user(securityToken)));
        time(&lastWelcome_);
    }
}

void AI::say(const std::string& textToSpeak, const bool& text_only)
{
    log.Info("Speech::say - " + textToSpeak);

    // Test mute state
    if (config_->is_mute || text_only) {
        return;
    }
    voice_->say_async(textToSpeak);
}

//! Toogle the mute state
bool AI::toggleMute()
{
    config_->is_mute = !config_->is_mute;
    return config_->is_mute;
}

//////////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////////

void AI::start()
{
    say(lexicon_manager::get_string(trad_key::AI_INITIATED, config_->name));
    //say(lexicon_manager::get_string(trad_key::AI_SELF_INTRODUCTION, config_->name));
}

void AI::shutdown()
{
    auto text = lexicon_manager::get_string(trad_key::AI_SHUTDOWN, config_->name);
    say(text);
}
