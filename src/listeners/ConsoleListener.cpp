#include "ConsoleListener.h"

#include <iostream>

ConsoleListener::ConsoleListener()
        : Listener("ConsoleListener.log")
{
    log_.Info("******************************************************");
    log_.Info("*****              ConsoleListener               *****");
    log_.Info("******************************************************");
}

ConsoleListener::~ConsoleListener()
{
    log_.Info("******************************************************");
    log_.Info("*****              ~ConsoleListener              *****");
    log_.Info("******************************************************");
}

void ConsoleListener::doStart()
{
    log_.Info("Do start");
    continueToListen_ = true;

    std::string entry;
    while (continueToListen_) {
        // TODO: Store messages in a message_queue instead of wainting for cin.
        std::cout << "Waiting for user command : " << std::endl;
        std::cin >> entry;
        log_.Debug("Command received : " + entry);

        // if entry == "exit", stop listening
        continueToListen_ = entry != "exit";

        // TODO: Add message : "command" is not supported
        // TODO: support "Kill" command
    }
    log_.Debug("EXIT !");
}