#include "Listener.h"

Listener::Listener() { }

Listener::~Listener() { }

/** Start the listening thread */
std::thread Listener::start()
{
	return std::thread(&Listener::doStart, this);
}

/** Stop the listener loop. */
void Listener::Stop()
{
	m_continueToListen = false;
}

// TODO: Create a callback to AI via open port.