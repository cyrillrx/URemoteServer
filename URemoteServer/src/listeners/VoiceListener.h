#pragma once

#include <sapi.h>

#include "Listener.h"
#include "Logger.h"
#include "..\helpers\ComHelper.h"

class VoiceListener : public Listener
{
public:
	VoiceListener();
	~VoiceListener();

private:
	Logger* m_log;

	void doStart();
	void listen();
	ISpRecoGrammar* initGrammar(ComHelper& helper, ISpRecoContext* recoContext);
	void getText(ComHelper& helper, ISpRecoContext* reco_context);
};
