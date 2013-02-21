#pragma once

#include <thread>

#include "Logger.h"

class Listener
{
public:
	Listener(void);
	~Listener(void);

	std::thread start();
	void stop();

protected:
	bool m_continueToListen;
	Logger* m_log;

private:
	virtual void doStart() = 0;
	//virtual void handleMessage() = 0;
};

