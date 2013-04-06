#pragma once

#include <string>

class Speech
{

public:
	Speech(const std::string& lang, const std::string& gender);
	~Speech();

	void sayInThread(const std::string& textToSpeak);

private:
	std::string language_;
	std::string gender_;

	void say(const std::string& textToSpeak);
};
