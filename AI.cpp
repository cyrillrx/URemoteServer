#include "AI.h"
#include "Speech.h"

#define DELAY 60*5 // 5 min / 300 sec before repeate time

//////////////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////////////
AI* AI::s_Instance = NULL;

AI* AI::GetInstance()
{
	if (!s_Instance) {
		s_Instance = new AI("Eternity");
	}
	
	return s_Instance;
}

void AI::FreeInstance()
{
	delete(s_Instance);
	s_Instance = NULL;
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
		Say("Welcome to " + m_Name + " sir." );
		time(&m_LastWelcome);
	}
}

void AI::Say(string _textToSpeak)
{
	// Test mute state
	if (!m_IsMute) {
		Speech::SayInThread(_textToSpeak);
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

AI::~AI(void)
{
	Shutdown();
}

void AI::Start()
{
	Say("Artificial Intelligence initiated.");
}

void AI::Shutdown()
{
	Say(m_Name + " is shutting down. Goodbye sir");

}
