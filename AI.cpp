#include "AI.h"
#include "Speech.h"

#define DELAY 600 // 600 sec (10 min) avant de répéter

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
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

void AI::Start()
{
	Say("Server activated. " + m_Name + " is online." );
}

void AI::Shutdown()
{
	Say(m_Name + " is shutting down. Goodbye sir" );
}
