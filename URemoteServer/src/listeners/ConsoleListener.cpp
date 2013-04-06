#include "ConsoleListener.h"

#include <string>
#include <iostream>

ConsoleListener::ConsoleListener()
{
	log_ = logger("ConsoleListener.log");
	log_.set_log_severity_console(logger::SEVERITY_LVL_WARNING);

	log_.info("******************************************************");
	log_.info("*****              ConsoleListener               *****");
	log_.info("******************************************************");
}

ConsoleListener::~ConsoleListener()
{
	log_.info("******************************************************");
	log_.info("*****              ~ConsoleListener              *****");
	log_.info("******************************************************");
}

void ConsoleListener::doStart()
{
	log_.info("Do start");
	continueToListen_ = true;

	std::string entry;
	while (continueToListen_) {
		// TODO: Store messages in a message_queue instead of wainting for cin.
		std::cout << "Waiting for user command : " << std::endl;
		std::cin >> entry;
		log_.debug("Command received : " + entry);
		
		// if entry == "exit", stop listening
		continueToListen_ = entry != "exit";

		// TODO: Add message : "command" is not supported
		// TODO: support "Kill" command
	}
	log_.debug("EXIT !");
}