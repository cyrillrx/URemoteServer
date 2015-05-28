#pragma once

#include <thread>

#include "logger/logger_manager.h"

class Listener
{
public:
	Listener();
	virtual ~Listener() = 0;

	std::thread start();
	void stop();

protected:
	bool continueToListen_;
	LoggerManager log_;

private:
	virtual void doStart() = 0;
	//virtual void handleMessage() = 0;
};

