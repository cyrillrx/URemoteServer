#include "text_to_speech.h"

#include <Windows.h>
#include <iostream>

#include "string_utils.h"

BOOL CALLBACK EnumUILanguagesProc(LPTSTR wLanguage, LONG_PTR lParam);

static std::vector<std::string> languages;

// TODO: clean function
std::vector<std::string> text_to_speech::available_languages()
{
	languages = std::vector<std::string>();
	if(FAILED(EnumUILanguages(EnumUILanguagesProc, MUI_LANGUAGE_NAME | MUI_USE_SEARCH_ALL_LANGUAGES, 0))) {
		std::cout << GetLastError() << std::endl;
	}
	return languages;
}

BOOL CALLBACK EnumUILanguagesProc(LPTSTR  wLanguage, LONG_PTR lParam)
{
	const auto language = string_utils::wstring_to_string(std::wstring(wLanguage));
	std::cout << "Langues : " << language << std::endl;
	languages.push_back(language);
	return TRUE;
}