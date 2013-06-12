#pragma once

#include <string>
#include "properties.h"
#include "voice.h"

class ai_config : public properties
{
public:
	ai_config(const std::string& path);
	~ai_config();
	const text_to_speech::language_code language_code();

	std::string name;
	std::string language;
	std::string gender;
	bool is_mute;
	long rate;
	

	static const std::string DEFAULT_LANG;

private:
	static const std::string KEY_NAME;
	static const std::string KEY_LANG;
	static const std::string KEY_GENDER;
	static const std::string KEY_MUTE;
	static const std::string KEY_RATE;

	static const std::string DEFAULT_NAME;
	static const std::string DEFAULT_GENDER;
	static const bool DEFAULT_MUTE;
	static const long DEFAULT_RATE;
};