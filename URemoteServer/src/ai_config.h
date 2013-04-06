#pragma once

#include <string>
#include "properties.h"

class ai_config : public properties
{
public:
	ai_config(const std::string& path);
	~ai_config();
	
	std::string name;
	std::string language;
	std::string gender;
	bool is_mute;

	static const std::string DEFAULT_LANG;

private:
	static const std::string KEY_NAME;
	static const std::string KEY_LANG;
	static const std::string KEY_GENDER;
	static const std::string KEY_MUTE;

	static const std::string DEFAULT_NAME;
	static const std::string DEFAULT_GENDER;
	static const bool DEFAULT_MUTE;
};