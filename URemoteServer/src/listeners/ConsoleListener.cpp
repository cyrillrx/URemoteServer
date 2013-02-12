#include "ConsoleListener.h"

#include <string>
#include <iostream>

using namespace std;

ConsoleListener::ConsoleListener(void)
{
	m_log = new Logger("ConsoleListener.log");
}

ConsoleListener::~ConsoleListener(void)
{
	if (m_log) {
		delete(m_log);
		m_log = nullptr;
	}
}

void ConsoleListener::doStart()
{
	m_log->info("******************************************************");
	m_log->info("*****           ConsoleListener Started          *****");
	m_log->info("******************************************************");

	m_continueToListen = true;

	string entry;
	bool continueToListen = true;
	while (m_continueToListen) {
		cout << "Waiting for user command : ";
		cin >> entry;
		m_log->debug("You entered " + entry);
		continueToListen = entry != "exit";
	}
	m_log->debug("EXIT !");
}