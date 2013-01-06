#include "AI.h"

#include "modules\Speech.h"
#include "Translator.h"
#include "TextKey.h"

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

bool AI::StartConnection(unique_ptr<ServerConfig> serverConfig)
{
	bool result = true;
	m_ExchangeServer = unique_ptr<Server>(new Server(move(serverConfig), this));
	
	// TODO: clean the function
	//Say(m_Name + " is now online.");
	auto translator = Translator::getInstance();
	auto text = translator->getString(TextKey::AI_SERVER_ONLINE, m_Config->Name);
	Say(text);

	result = m_ExchangeServer->Start();
	
	return result;
}

bool AI::StopConnection()
{
	if (m_ExchangeServer) {
		m_ExchangeServer->Stop();
		return true;
	}

	return false;
}

void AI::Welcome()
{
	// Calculate the elapsed time since the last call to the method
	time_t now;
	time(&now);
	cout << "now " << now << endl;
	auto elapsedTime = difftime(now, m_LastWelcome);
	cout << "elapsedTime " << elapsedTime << endl;

	// Welcome if last welcome > 10 min
	if (elapsedTime > DELAY) {
		// TODO: clean the function
		//Say("Welcome to " + m_Config->Name + " sir.");
		auto translator = Translator::getInstance();
		auto text = translator->getString(TextKey::AI_WELCOME, m_Config->Name);
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
	//Say("Artificial Intelligence initiated.");
	// TODO: clean the function
	auto translator = Translator::getInstance();
	auto text = translator->getString(TextKey::AI_INITIATED);
	Say(text);
}

void AI::Shutdown()
{
	//Say(m_Config->Name + " is shutting down. Goodbye sir");
	// TODO: clean the function
	auto translator = Translator::getInstance();
	auto text = translator->getString(TextKey::AI_SHUTDOWN, m_Config->Name);
	Say(text);
}