#pragma once

#include <string>
using namespace std;

class AI
{
private:
	int m_IsMute;
	int m_LastWelcome;

public:
	AI(void);
	~AI(void);
	void Welcome();
	void Say(string _textToSpeak);
};

