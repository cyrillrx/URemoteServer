#pragma once

#include "Listener.h"
#include "../Exchange.h"

class VoiceListener : public Listener
{
public:
	VoiceListener(AI *ai);
	virtual ~VoiceListener();

private:
	virtual void doStart();
	AI * ai_;

	void listen();
};
