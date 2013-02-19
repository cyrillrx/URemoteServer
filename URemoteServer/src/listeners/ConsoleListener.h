#pragma once

#include "Listener.h"
#include "Logger.h"

class ConsoleListener : public Listener
{
public:
	ConsoleListener();
	~ConsoleListener();

private:
	Logger* m_log;

	void doStart();
};
