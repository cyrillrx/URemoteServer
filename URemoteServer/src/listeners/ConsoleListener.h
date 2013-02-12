#pragma once

#include "Listener.h"
#include "Logger.h"

class ConsoleListener : public Listener
{
public:
	ConsoleListener(void);
	~ConsoleListener(void);

private:
	Logger* m_log;

	void doStart();
};

