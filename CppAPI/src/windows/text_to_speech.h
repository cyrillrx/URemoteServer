#pragma once

#include <vector>
#include <string>

#include <comdef.h>
#include <sphelper.h>

namespace text_to_speech
{
	void initVoice(ISpVoice * ispVoice, const std::string& language, const std::string& gender);

	const wchar_t* langToAttribute(std::string language);
	const wchar_t* genderToAttribute(std::string gender);
}

