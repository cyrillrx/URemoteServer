#include "text_to_speech.h"

#include <iostream>
#include <Windows.h>
#include "helpers\ComHelper.h"
#include "exception\config_exception.h"
#include "Utils.h"
#include "string_utils.h"

std::vector<std::string> text_to_speech::available_languages()
{
	// TODO: Find a way to implement text_to_speech::available_languages()
	return std::vector<std::string>();
}

/**
* Try to initialize a voice object to check if the properties are OK.
* @return True if parameters are correct, false othewise.
* @throws and Exception if a problem occurs.
*/
bool text_to_speech::testParameters(const std::string& language, const std::string& gender)
{
	// Init COM lib
	ComHandler comHandler(*Utils::getLogger());
	HRESULT hr;
	ISpVoice * ispVoice = nullptr;
	hr = ::CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, reinterpret_cast<void**>(&ispVoice));
	ComHelper::checkResult("Speech::testParameters", hr);

	try {
		initVoice(ispVoice, language, gender);
		return true;
	} catch (const software_exception&) {
		
	}
	return false;
}

void text_to_speech::initVoice(ISpVoice * ispVoice, const std::string& language, const std::string& gender)
{
	const wchar_t* reqAttributs = langToAttribute(language);
	const wchar_t* optAttributs = genderToAttribute(gender);

	ISpObjectToken* cpTokenEng;
	if (FAILED(::SpFindBestToken(SPCAT_VOICES, reqAttributs, optAttributs, &cpTokenEng))) { 
		throw software_exception("Speech::initVoice", "Couldn't find a Token with the required attributs.");
	}

	ispVoice->SetVoice(cpTokenEng);
	//TODO: Config Rate with file
	ispVoice->SetRate(long(0.5));
}

const wchar_t* text_to_speech::langToAttribute(std::string language)
{
	// TODO: initialize Language in a map
	// Default value : 409 = English 
	const wchar_t* defaultValue = L"Language=409";

	if (language == "FR") {
		return L"Language=40C";
	}

	return defaultValue;
}

// TODO: initialize Gender in a map
const wchar_t* text_to_speech::genderToAttribute(std::string gender)
{
	const wchar_t* defaultValue = L"Gender=Female";

	if (gender == "M") {
		return L"Gender=Male";
	}

	return defaultValue;
}