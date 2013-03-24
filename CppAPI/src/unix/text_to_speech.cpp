#include "platform_config.h"
#if defined(LINUX_PLATFORM)

#include "../text_to_speech.h"

std::vector<std::string> text_to_speech::available_languages()
{
    // TODO: Find a way to implement text_to_speech::available_languages()
	return std::vector<std::string>();
}

bool text_to_speech::testParameters(const std::string& language, const std::string& gender)
{
    // TODO: Implement text_to_speech::testParameters() for Linux
    return false;
}

bool text_to_speech::say(const std::string& textToSpeak, const std::string& language, const std::string& gender)
{
    // TODO: Implement text_to_speech::say() for Linux
    return false;
}

#endif
