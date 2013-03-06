#pragma once

#include <string>
#include <iostream>
#include <comdef.h>
#include <sphelper.h>

class Speech
{

public:
	Speech(const std::string& lang, const std::string& gender);
	~Speech();

	void sayInThread(const std::string& textToSpeak);

private:
	std::string m_language;
	std::string m_gender;

	void testParameters();
	void initVoice(ISpVoice * ispVoice);
	void say(const std::string& textToSpeak);
	bool sayB(const bstr_t& textToSpeak);
};