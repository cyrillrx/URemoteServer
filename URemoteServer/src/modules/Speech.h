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
	std::string m_Language;
	std::string m_Gender;
	
	static void initVoice(ISpVoice * ispVoice, const std::string& language, const std::string& gender);
	static void Say(const std::string& textToSpeak, const std::string& language, const std::string& gender);
	static void SayB(const bstr_t& textToSpeak, const std::string& language, const std::string& gender);
};