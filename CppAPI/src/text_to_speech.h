#pragma once

#include <vector>
#include <string>
#include <comdef.h>
#include <sphelper.h>

namespace text_to_speech
{
	std::vector<std::string> available_languages();
	void testParameters(const std::string& language, const std::string& gender);
	void initVoice(ISpVoice * ispVoice, const std::string& language, const std::string& gender);
};

