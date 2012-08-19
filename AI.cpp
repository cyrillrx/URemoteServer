#include "AI.h"
#include "Speech.h"

#define DELAY 60*5 // 5 min / 300 sec avant de répéter

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

AI* AI::GetInstance()
{
	if (!s_Instance) {
		s_Instance = new AI("Eternity");
	}
	
	return s_Instance;
}

AI::~AI(void)
{
	Shutdown();
}

bool AI::StartConnection(int _port, int _maxConnections)
{
	bool result = true;
	m_ExchangeServer = new Server(_port, _maxConnections);
	Say(m_Name + " is now online.");

	result = m_ExchangeServer->Start();
	
	delete(m_ExchangeServer);
	m_ExchangeServer = NULL;

	return result;
}

bool AI::StopConnection()
{
	if (!m_ExchangeServer) {
		return false;
	}
	m_ExchangeServer->Stop();

	return true;
}

void AI::Welcome()
{
	// Welcome if last welcom > 10 min
	
	// Calcule du temps écoulé en minutes depuis le dernier appel à la fonction
	time_t now;
	time(&now);
	cout << "now " << now << endl;
	double elapsedTime = difftime(now, m_LastWelcome);
	cout << "elapsedTime " << elapsedTime << endl;
	if (elapsedTime > DELAY) {
		Say("Cyril is now connected. Welcome to " + m_Name + " sir." );
		time(&m_LastWelcome);
	}
}

void AI::Say(string _textToSpeak)
{
	// Test mute
	if (!m_IsMute) {
		Speech::Say(_textToSpeak);
	}
}

//! Change l'état de mute et renvoie le nouvel état
bool AI::ToggleMute()
{
	m_IsMute = !m_IsMute;
	return m_IsMute;
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

AI::AI(string _name)
{
	m_Name = _name;
	m_IsMute = false;

	time(&m_LastWelcome);
	m_LastWelcome -= DELAY;

	Start();
}

void AI::Start()
{
	Say("A.I. activated.");
}

void AI::Shutdown()
{
	Say(m_Name + " is shutting down. Goodbye sir");

}
