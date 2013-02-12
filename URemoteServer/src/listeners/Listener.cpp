#include "Listener.h"

using namespace std;

Listener::Listener(void)
{
}

Listener::~Listener(void)
{
}

/** Start the listening thread */
thread Listener::start()
{
	return thread(&Listener::doStart, this);
}

/** Stop the listener loop. */
void Listener::Stop()
{
	m_continueToListen = false;
}
