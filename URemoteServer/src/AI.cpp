#include "AI.h"

#include <thread>
#include "modules\Speech.h"
#include "Translator.h"
#include "TextKey.h"
#include "Utils.h"

#define DELAY 60*5 // 5 min / 300 sec before repeate time

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////
AI::AI(unique_ptr<AIConfig> config) : m_Config(move(config))
{
	m_Voice = unique_ptr<Speech>(new Speech(m_Config->Lang, m_Config->Gender));
	time(&m_LastWelcome);
	m_LastWelcome -= DELAY;

	Start();
}

AI::~AI()
{
	Shutdown();
}

void AI::StartConnection(unique_ptr<ServerConfig> serverConfig)
{
	// TODO: Instanciate other listeners
	thread uRemoteThread;
	thread consoleThread;
	//thread uiListener;

	// TODO: Change debug messages
	try {
		m_uRemoteListener = unique_ptr<URemoteListener>(new URemoteListener(move(serverConfig), this));
		uRemoteThread = m_uRemoteListener->start();
		Utils::getLogger()->debug("AI::StartConnection(), URemoteListener OK");

	} catch (const exception&) {
		Utils::getLogger()->error("AI::StartConnection(), URemoteListener KO");
	}

	try {
		m_consoleListener = unique_ptr<ConsoleListener>(new ConsoleListener());
		consoleThread = m_consoleListener->start();
		Utils::getLogger()->debug("AI::StartConnection(), ConsoleListener OK");

	} catch (const exception&) {
		Utils::getLogger()->error("AI::StartConnection(), ConsoleListener KO");
	}

	// Notify the user that the listener are open.
	auto text = Translator::getString(TextKey::AI_SERVER_ONLINE, m_Config->Name);
	Say(text);

	// Join the listener threads
	uRemoteThread.join();
	consoleThread.join();
}

void AI::StopConnection()
{
	if (m_uRemoteListener) {
		m_uRemoteListener->Stop();
	}
	if (m_consoleListener) {
		m_consoleListener->Stop();
	}
}

void AI::Welcome()
{
	// Calculate the elapsed time since the last call to the method
	time_t now;
	time(&now);
	Utils::getLogger()->debug("AI::Welcome() now " + to_string(now));
	auto elapsedTime = difftime(now, m_LastWelcome);
	Utils::getLogger()->debug("AI::Welcome() elapsedTime " + to_string(elapsedTime));

	// Welcome if last welcome > DELAY
	if (elapsedTime > DELAY) {
		auto text = Translator::getString(TextKey::AI_WELCOME, m_Config->Name);
		Say(text);
		time(&m_LastWelcome);
	}
}

void AI::Say(string textToSpeak)
{
	// Test mute state
	if (!m_Config->IsMute) {
		m_Voice->SayInThread(textToSpeak);
	}
}

//! Change l'état de mute et renvoie le nouvel état
bool AI::ToggleMute()
{
	m_Config->IsMute = !m_Config->IsMute;
	return m_Config->IsMute;
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

void AI::Start()
{
	auto text = Translator::getString(TextKey::AI_INITIATED);
	Say(text);
}

void AI::Shutdown()
{
	// TODO: clean the function
	auto text = getString(TextKey::AI_SHUTDOWN, m_Config->Name);
	//auto text = Translator::getString(TextKey::AI_SHUTDOWN, m_Config->Name);
	Say(text);
}