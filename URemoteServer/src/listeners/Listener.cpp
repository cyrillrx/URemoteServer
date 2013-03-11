#include "Listener.h"

#include "exception\Exception.h"

Listener::Listener() : m_log("listener.log") { }

Listener::~Listener() { }

/** Start the listening thread */
std::thread Listener::start()
{
	return std::thread(&Listener::doStart, this);
}

/** Stop the listener loop. */
void Listener::stop()
{
	m_continueToListen = false;
}

// TODO: Create a callback to AI via open port.