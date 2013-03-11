#include "ConsoleListener.h"

#include <string>
#include <iostream>

ConsoleListener::ConsoleListener()
{
	m_log = logger("ConsoleListener.log");
	m_log.setLogSeverityConsole(logger::SEVERITY_LVL_WARNING);

	m_log.info("******************************************************");
	m_log.info("*****              ConsoleListener               *****");
	m_log.info("******************************************************");
}

ConsoleListener::~ConsoleListener()
{
	m_log.info("******************************************************");
	m_log.info("*****              ~ConsoleListener              *****");
	m_log.info("******************************************************");
}

void ConsoleListener::doStart()
{
	m_log.info("Do start");
	m_continueToListen = true;

	std::string entry;
	while (m_continueToListen) {
		std::cout << "Waiting for user command : " << std::endl;
		std::cin >> entry;
		m_log.debug("Command received : " + entry);
		
		// if entry == "exit", stop listening
		m_continueToListen = entry != "exit";

		// TODO: Add message : "command" is not supported
		// TODO: support "Kill" command
	}
	m_log.debug("EXIT !");
}