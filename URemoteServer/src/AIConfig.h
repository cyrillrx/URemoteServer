#pragma once

#include <string>
#include "Properties.h"

class AIConfig : public Properties
{
public:
	AIConfig(const std::string& path);
	~AIConfig();
	
	std::string Name;
	std::string Lang;
	std::string Gender;
	bool IsMute;

private:
	static const std::string KEY_NAME;
	static const std::string KEY_LANG;
	static const std::string KEY_GENDER;
	static const std::string KEY_MUTE;

	static const std::string DEFAULT_NAME;
	static const std::string DEFAULT_LANG;
	static const std::string DEFAULT_GENDER;
	static const bool DEFAULT_MUTE;
};