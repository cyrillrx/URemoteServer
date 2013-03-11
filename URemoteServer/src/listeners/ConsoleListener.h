#pragma once

#include "Listener.h"

class ConsoleListener : public Listener
{
public:
	ConsoleListener();
	virtual ~ConsoleListener();

private:
	virtual void doStart();
};
