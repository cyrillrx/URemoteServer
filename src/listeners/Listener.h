#pragma once

#include <thread>

#include "logger/logger_manager.h"

static const std::string DEFAULT_FILENAME = "listener.log";

class Listener
{
public:
	Listener(const std::string& logPath = DEFAULT_FILENAME);
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

