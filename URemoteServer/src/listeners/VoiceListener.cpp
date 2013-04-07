#include "VoiceListener.h"

#include "lexicon_manager.h"
#include "voice_recognition.h"
#include "../trad_key.h"

VoiceListener::VoiceListener(std::shared_ptr<AI> ai)
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
	/*
	recognizer.add_vocal_command();
	ai_->say(lexicon_manager::get_string(trad_key::AI_YES));}

	recognizer.start_listening();
	*/
}