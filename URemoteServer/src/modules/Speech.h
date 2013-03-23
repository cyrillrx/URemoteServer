#pragma once

#include <string>
#include <iostream>

#include "platform_config.h"
# if defined(WINDOWS_PLATFORM)
#include <comdef.h>
#include <sphelper.h>
# endif

class Speech
{

public:
	Speech(const std::string& lang, const std::string& gender);
	~Speech();

	void sayInThread(const std::string& textToSpeak);

private:
	std::string m_language;
	std::string m_gender;

	void say(const std::string& textToSpeak);
# if defined(WINDOWS_PLATFORM)
	bool sayB(const bstr_t& textToSpeak);
# endif
};
