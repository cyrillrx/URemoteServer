#pragma once

class Listener
{
public:
	Listener(void);
	~Listener(void);
	virtual void start() = 0;

private:
	void doStart();
};

