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
	bool continueToListen_;
	logger log_;

private:
	virtual void doStart() = 0;
	//virtual void handleMessage() = 0;
};

