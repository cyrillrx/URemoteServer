#pragma once

#include <string>
// TODO: Delete Speech class. Replace by cpp_cxp_api voice
class Speech
{

public:
	Speech(const std::string& lang, const std::string& gender, const long& rate);
	~Speech();

	void sayInThread(const std::string& textToSpeak);

private:
	std::string language_;
	std::string gender_;
	long rate_;

	void say(const std::string& textToSpeak);
};
