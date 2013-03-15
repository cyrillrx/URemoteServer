#pragma once

#include <vector>
#include <string>
#include <comdef.h>
#include <sphelper.h>

namespace text_to_speech
{
	namespace languague
	{
		const std::string EN = "EN";
		const std::string FR = "FR";
	};

	const std::string default_lang = languague::EN;

	std::vector<std::string> available_languages();
	bool testParameters(const std::string& language, const std::string& gender);
	void initVoice(ISpVoice * ispVoice, const std::string& language, const std::string& gender);

	const wchar_t* langToAttribute(std::string language);
	const wchar_t* genderToAttribute(std::string gender);
};

