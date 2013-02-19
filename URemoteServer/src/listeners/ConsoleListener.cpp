#include "ConsoleListener.h"

#include <string>
#include <iostream>

ConsoleListener::ConsoleListener()
{
	m_log = new Logger("ConsoleListener.log");
	m_log->setLogSeverityConsole(Logger::SEVERITY_LVL_WARNING);
}

ConsoleListener::~ConsoleListener()
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

	std::string entry;
	while (m_continueToListen) {
		std::cout << "Waiting for user command : " << std::endl;
		std::cin >> entry;
		m_log->debug("Command received : " + entry);
		
		// if entry == "exit", stop listening
		m_continueToListen = entry != "exit";

		// TODO: Add message : "command" is not supported
		// TODO: support "Kill" command
	}
	m_log->debug("EXIT !");
}