#pragma once

#include "Listener.h"

class ConsoleListener : public Listener
{
public:
	ConsoleListener(void);
	~ConsoleListener(void);

private:
	void doStart();
};

