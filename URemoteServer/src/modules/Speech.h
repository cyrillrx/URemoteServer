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

	void SayInThread(const std::string& textToSpeak);

private:
	std::string m_language;
	std::string m_gender;
	
	void initVoice(ISpVoice * ispVoice);
	void Say(const std::string& textToSpeak);
	void SayB(const bstr_t& textToSpeak);
};