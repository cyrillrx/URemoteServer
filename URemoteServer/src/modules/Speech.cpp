#include "Speech.h"

#include <thread>
#include "text_to_speech.h"
#include "Utils.h"

Speech::Speech(const std::string& lang, const std::string& gender)
	: language_(lang), gender_(gender) { }

Speech::~Speech() { }

void Speech::say(const std::string& textToSpeak)
{
	if (text_to_speech::say(textToSpeak, language_, gender_)) {
		Utils::get_logger()->info("Speech::say - " + textToSpeak);
	}
}

void Speech::sayInThread(const std::string& textToSpeak)
{
	std::thread speakThread(&Speech::say, this, textToSpeak);
	speakThread.join();
}
