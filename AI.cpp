#include "AI.h"
#include "Speech.h"

AI::AI(string _name)
{
	m_Name = _name;
	m_IsMute = false;
	time(&m_LastWelcome);
	m_LastWelcome -= 60 * 10; // - 10 minutes

}

AI::~AI(void)
{
}

void AI::Welcome()
{
	// Welcome if last welcom > 10 min
	
	// Calcule du temps écoulé en minutes depuis le dernier appel à la fonction
	time_t now;
	time(&now);
	double elapsedTime = difftime(m_LastWelcome, now) / 60.0;
	if (elapsedTime > 10) {
		Say("Welcome to " + m_Name + " sir." );
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
