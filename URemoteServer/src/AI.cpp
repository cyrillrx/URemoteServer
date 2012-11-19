#include "AI.h"

#include "modules\Speech.h"

#define DELAY 60*5 // 5 min / 300 sec before repeate time

//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////
AI::AI(unique_ptr<AIConfig> config) : m_Config(move(config))
{
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
	
	//Say(m_Name + " is now online.");
	Say(m_Config->Name + " est maintenant en ligne.");

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
	double elapsedTime = difftime(now, m_LastWelcome);
	cout << "elapsedTime " << elapsedTime << endl;

	// Welcome if last welcome > 10 min
	if (elapsedTime > DELAY) {
		Say("Welcome to " + m_Config->Name + " sir." );
		time(&m_LastWelcome);
	}
}

void AI::Say(string textToSpeak)
{
	// Test mute state
	if (!m_Config->IsMute) {
		Speech::SayInThread(textToSpeak);
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
	Say("Intelligence artificielle initialisée.");
}

void AI::Shutdown()
{
	Say(m_Config->Name + " is shutting down. Goodbye sir");
}