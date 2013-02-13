#include "Listener.h"

Listener::Listener(void)
{
}

Listener::~Listener(void)
{
}

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
