#include "Listener.h"

using namespace std;

Listener::Listener(void)
{
}

Listener::~Listener(void)
{
}

thread Listener::start()
{
	return thread(&Listener::doStart, this);
}

// thread Listener::doStart() { }
