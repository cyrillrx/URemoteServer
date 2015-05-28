#include "VoiceListener.h"

#include "lang/lexicon_manager.h"
#include "../trad_key.h"

// TODO update Biicode config
//#include "voice_recognition.h"
#include "cyrillrx/cross_api/src/voice_recognition.h"

VoiceListener::VoiceListener(std::shared_ptr<AI> ai)
	: Listener("VoiceListener.log"), ai_(ai)
{
	log_.Info("******************************************************");
	log_.Info("*****                 VoiceListener              *****");
	log_.Info("******************************************************");
}

VoiceListener::~VoiceListener()
{
	log_.Info("******************************************************");
	log_.Info("*****                ~VoiceListener              *****");
	log_.Info("******************************************************");
}

void VoiceListener::doStart()
{
	log_.Info("Do start");
	continueToListen_ = voice_recognition::is_implemented();

	std::string entry;
	while (continueToListen_) {

		// TODO: code voice recognition
		std::string keyword = "Eternity";
		log_.Info("Wait for " + keyword);
		listen(keyword);
		log_.Info("Do continue");
		// TODO: Add message : "command" is not supported
		// TODO: support "Kill" command
	}
	log_.Debug("EXIT !");
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