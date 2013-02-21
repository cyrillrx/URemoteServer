#pragma once

#include <sapi.h>

#include "Listener.h"
#include "..\Exchange.h"

class VoiceListener : public Listener
{
public:
	VoiceListener(AI *ai);
	~VoiceListener();

private:
	AI * m_ai;

	void doStart();
	void listen();
	ISpRecoGrammar* initGrammar(ISpRecoContext* recoContext);
	void getText(ISpRecoContext* reco_context);
};
