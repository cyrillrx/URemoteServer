#pragma once

#include "Listener.h"

// TODO: Change for UserInputListener
class ConsoleListener : public Listener
{
public:
    ConsoleListener();

    virtual ~ConsoleListener();

private:
    virtual void doStart();
};
