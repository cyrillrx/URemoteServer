#pragma once

#include <thread>

class Listener
{
public:
	Listener(void);
	~Listener(void);
	std::thread start();

private:
	virtual void doStart() = 0;
};

