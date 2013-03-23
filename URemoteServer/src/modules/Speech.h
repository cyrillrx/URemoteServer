#pragma once

#include <string>

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
};
