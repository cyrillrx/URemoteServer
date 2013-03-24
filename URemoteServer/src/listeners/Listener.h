#pragma once

#include <thread>

#include "logger.h"

class Listener
{
public:
	Listener();
	virtual ~Listener() = 0;

	std::thread start();
	void stop();

protected:
	bool m_continueToListen;
	logger m_log;

private:
	virtual void doStart() = 0;
	//virtual void handleMessage() = 0;
};

