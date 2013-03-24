#include "VoiceListener.h"

#include "../AI.h"
#include "../trad_key.h"
#include "Translator.h"
#include "voice_recognition.h"

VoiceListener::VoiceListener(AI *ai)
	: m_ai(ai)
{
	m_log = logger("VoiceListener.log");
	m_log.setLogSeverityConsole(logger::SEVERITY_LVL_WARNING);

	m_log.info("******************************************************");
	m_log.info("*****                 VoiceListener              *****");
	m_log.info("******************************************************");
}

VoiceListener::~VoiceListener()
{
	m_log.info("******************************************************");
	m_log.info("*****                ~VoiceListener              *****");
	m_log.info("******************************************************");
}

void VoiceListener::doStart()
{
	m_log.info("Do start");
	m_continueToListen = true;

	std::string entry;
	while (m_continueToListen) {

		// TODO: code voice recognition
		listen();
		// TODO: Add message : "command" is not supported
		// TODO: support "Kill" command
	}
	m_log.debug("EXIT !");
}

void VoiceListener::listen()
{
	voice_recognition::recognizer recognizer;
	
	recognizer.add_vocal_command();
	// m_ai->say(Translator::getString(trad_key::AI_YES));}

	recognizer.start_listening();
}