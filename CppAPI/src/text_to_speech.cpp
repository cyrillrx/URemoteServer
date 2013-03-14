#include "text_to_speech.h"

#include <iostream>
#include <Windows.h>
#include "helpers\ComHelper.h"
#include "exception\Exception.h"
#include "Utils.h"
#include "string_utils.h"

//BOOL CALLBACK EnumUILanguagesProc(LPTSTR wLanguage, LONG_PTR lParam);
//
//static std::vector<std::string> languages;
//
//// TODO: clean function text_to_speech::available_languages()
//std::vector<std::string> text_to_speech::available_languages()
//{
//	languages = std::vector<std::string>();
//	if(FAILED(EnumUILanguages(EnumUILanguagesProc, MUI_LANGUAGE_NAME | MUI_USE_SEARCH_ALL_LANGUAGES, 0))) {
//		std::cout << GetLastError() << std::endl;
//	}
//	return languages;
//}
//
//BOOL CALLBACK EnumUILanguagesProc(LPTSTR  wLanguage, LONG_PTR lParam)
//{
//	const auto language = string_utils::wstring_to_string(std::wstring(wLanguage));
//	std::cout << "Langues : " << language << std::endl;
//	languages.push_back(language);
//	return TRUE;
//}

std::vector<std::string> text_to_speech::available_languages()
{
	return std::vector<std::string>();
}

/**
* Try to initialize a voice object to check if the properties are OK.
* @throws and Exception if a problem occurs.
*/
void text_to_speech::testParameters(const std::string& language, const std::string& gender)
{
	// Init COM lib
	ComHandler comHandler(*Utils::getLogger());
	HRESULT hr;
	ISpVoice * ispVoice = nullptr;
	hr = ::CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, reinterpret_cast<void**>(&ispVoice));
	ComHelper::checkResult("Speech::testParameters", hr);
	initVoice(ispVoice, language, gender);
}

void text_to_speech::initVoice(ISpVoice * ispVoice, const std::string& language, const std::string& gender)
{
	// TODO: initialize Language and gender in a map
	const wchar_t* reqAttributsDefault = L"Language=409";
	const wchar_t* reqAttributs = (language == "FR")	? L"Language=40C" : reqAttributsDefault;
	const wchar_t* optAttributs = (gender == "M")		? L"Gender=Male"  : L"Gender=Female";

	ISpObjectToken* cpTokenEng;
	HRESULT hr = ::SpFindBestToken(SPCAT_VOICES, reqAttributs, optAttributs, &cpTokenEng);
	if (FAILED(hr)) {
		throw Exception("SoftwareException", "Speech::initVoice", "Couldn't find a Token with the required attributs.");
	}
	ispVoice->SetVoice(cpTokenEng);
	//TODO: Config Rate with file
	ispVoice->SetRate(long(0.5));
}