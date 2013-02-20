#pragma once

#include <thread>

class Listener
{
public:
	Listener(void);
	~Listener(void);

	std::thread start();
	void stop();

protected:
	bool m_continueToListen;

private:
	virtual void doStart() = 0;
};

