#pragma once

#include "Listener.h"
#include "Logger.h"

class VoiceListener : public Listener
{
public:
	VoiceListener();
	~VoiceListener();

private:
	Logger* m_log;

	void doStart();
};
