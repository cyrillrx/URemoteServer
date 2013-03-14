#include "Speech.h"

#include <thread>
#include "text_to_speech.h"
#include "helpers\ComHelper.h"
#include "Utils.h"

Speech::Speech(const std::string& lang, const std::string& gender)
	: m_language(lang), m_gender(gender)
{
}

Speech::~Speech() { }

bool Speech::sayB(const bstr_t& textToSpeak)
{
	// Init COM lib
	ComHandler comHandler(*Utils::getLogger());
	HRESULT hr;

	ISpVoice * ispVoice = nullptr;

	bool result = false;

	hr = ::CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, reinterpret_cast<void**>(&ispVoice));
	ComHelper::checkResult("Speech::sayB", hr);

	if (SUCCEEDED( hr )) {

		text_to_speech::initVoice(ispVoice, m_language, m_gender);

		hr = ispVoice->Speak(textToSpeak, SPF_ASYNC, nullptr);
		if (hr == S_OK) {
			result = true;

		} else if (hr == E_INVALIDARG) {
			Utils::getLogger()->error("Speech::SayB - One or more parameters are invalid.");

		} else if (hr == E_POINTER) {
			Utils::getLogger()->error("Speech::SayB - Invalid pointer.");

		} else if (hr == E_OUTOFMEMORY) {
			Utils::getLogger()->error("Speech::SayB - Exceeded available memory.");

		} else {
			Utils::getLogger()->error("Speech::SayB - Unknown error.");
		}

		hr = ispVoice->WaitUntilDone(30000);

		ispVoice->Release();
		ispVoice = nullptr;
	}

	return result;
}

void Speech::say(const std::string& textToSpeak)
{
	bstr_t bstrTextToSpeak(textToSpeak.c_str());
	if (sayB(bstrTextToSpeak)) {
		Utils::getLogger()->info("Speech::say - " + textToSpeak);
	}
}

void Speech::sayInThread(const std::string& textToSpeak)
{
	std::thread speakThread(&Speech::say, this, textToSpeak);
	speakThread.join();
}