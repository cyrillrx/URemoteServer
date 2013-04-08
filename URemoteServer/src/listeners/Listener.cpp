#include "Listener.h"

#include "exception/Exception.h"

Listener::Listener() : log_("listener.log") { }

Listener::~Listener() { }

/** Start the listening thread */
std::thread Listener::start()
{
	return std::thread(&Listener::doStart, this);
}

/** Stop the listener loop. */
void Listener::stop()
{
	continueToListen_ = false;
}

// TODO: Create a callback to AI via open port.
