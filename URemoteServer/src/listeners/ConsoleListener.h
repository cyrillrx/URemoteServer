#pragma once

#include "Listener.h"

class ConsoleListener : public Listener
{
public:
	ConsoleListener();
	~ConsoleListener();

private:
	void doStart();
};
