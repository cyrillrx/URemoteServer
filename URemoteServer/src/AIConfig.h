#pragma once

#include <string>
#include "Properties.h"

class AIConfig : public Properties
{
public:
	AIConfig(const string& path);
	~AIConfig();
	
	string Name;
	bool IsMute;

private:
	static const string KEY_NAME;
	static const string KEY_MUTE;

	static const string DEFAULT_NAME;
	static const bool DEFAULT_MUTE;
};

