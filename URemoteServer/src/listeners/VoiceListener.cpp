#include "VoiceListener.h"

#include "../AI.h"
#include "../trad_key.h"
#include "lexicon_manager.h"
#include "voice_recognition.h"

VoiceListener::VoiceListener(AI *ai)
	: ai_(ai)
{
	log_ = logger("VoiceListener.log");
	log_.set_log_severity_console(logger::SEVERITY_LVL_WARNING);

	log_.info("******************************************************");
	log_.info("*****                 VoiceListener              *****");
	log_.info("******************************************************");
}

VoiceListener::~VoiceListener()
{
	log_.info("******************************************************");
	log_.info("*****                ~VoiceListener              *****");
	log_.info("******************************************************");
}

void VoiceListener::doStart()
{
	log_.info("Do start");
	continueToListen_ = true;

	std::string entry;
	while (continueToListen_) {

		// TODO: code voice recognition
		listen();
		// TODO: Add message : "command" is not supported
		// TODO: support "Kill" command
	}
	log_.debug("EXIT !");
}

void VoiceListener::listen()
{
	voice_recognition::recognizer recognizer;
	
	recognizer.add_vocal_command();
	// m_ai->say(lexicon_manager::getString(trad_key::AI_YES));}

	recognizer.start_listening();
}