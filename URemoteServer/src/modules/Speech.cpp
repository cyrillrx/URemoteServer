#include "Speech.h"

#include <thread>

#include "..\helpers\ComHelper.h"
#include "exception\Exception.h"
#include "Utils.h"

Speech::Speech(const std::string& lang, const std::string& gender)
	: m_language(lang), m_gender(gender)
{
	testParameters();
}

Speech::~Speech() { }

/**
* Try to initialize a voice object to check if the properties are OK.
* @throws and Exception if a problem occurs.
*/
void Speech::testParameters()
{
	// Init COM lib
	ComHandler comHandler(*Utils::getLogger());
	HRESULT hr;
	ISpVoice * ispVoice = nullptr;
	hr = ::CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, reinterpret_cast<void**>(&ispVoice));
	ComHelper::checkResult("Speech::testParameters", hr);
	initVoice(ispVoice);
}

void Speech::initVoice(ISpVoice * ispVoice)
{
	// TODO: initialize Language and gender in a map
	const wchar_t* reqAttributsDefault = L"Language=409";
	const wchar_t* reqAttributs = (m_language == "FR")	? L"Language=40C" : reqAttributsDefault;
	const wchar_t* optAttributs = (m_gender == "M")		? L"Gender=Male"  : L"Gender=Female";

	ISpObjectToken* cpTokenEng;
	HRESULT hr = ::SpFindBestToken(SPCAT_VOICES, reqAttributs, optAttributs, &cpTokenEng);
	if (FAILED(hr)) {
		throw Exception("SoftwareException", "Speech::initVoice", "Couldn't find a Token with the required attributs.");
	}
	ispVoice->SetVoice(cpTokenEng);
	//TODO: Config Rate with file
	ispVoice->SetRate(long(0.5));
}

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

		initVoice(ispVoice);

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