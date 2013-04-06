#pragma once

#include "platform_config.h"
#if defined(WINDOWS_PLATFORM)

#include <vector>
#include <string>

#include <comdef.h>
#include <sphelper.h>

namespace text_to_speech
{
	void init_voice(ISpVoice * ispVoice, const std::string& language, const std::string& gender);

	const wchar_t* lang_to_attribute(std::string language);
	const wchar_t* gender_to_attribute(std::string gender);
}

#endif
