#pragma once

#include <string>
#include <time.h>
using namespace std;

class AI
{
private:
	string m_Name;
	int m_IsMute;
	time_t m_LastWelcome;

	//double ElapsedTime(clock_t _clock1, clock_t _clock2);

public:
	AI(string _name);
	~AI(void);
	void Welcome();
	void Say(string _textToSpeak);
};

