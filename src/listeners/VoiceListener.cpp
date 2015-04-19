#include "VoiceListener.h"

#include "lang/lexicon_manager.h"
#include "../trad_key.h"

// TODO update Biicode config
//#include "voice_recognition.h"
#include "cyrillrx/cross_api/src/voice_recognition.h"

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
	continueToListen_ = voice_recognition::is_implemented();

	std::string entry;
	while (continueToListen_) {

		// TODO: code voice recognition
		std::string keyword = "Eternity";
		log_.info("Wait for " + keyword);
		listen(keyword);
		log_.info("Do continue");
		// TODO: Add message : "command" is not supported
		// TODO: support "Kill" command
	}
	log_.debug("EXIT !");
}

void VoiceListener::listen(const std::string& keyword)
{
	voice_recognition::recognizer recognizer;
	recognizer.start_listening(keyword);
	/*
	recognizer.add_vocal_command();
	ai_->say(lexicon_manager::get_string(trad_key::AI_YES));}

	recognizer.start_listening();
	*/
	ai_->say(lexicon_manager::get_string(trad_key::AI_YES));
}