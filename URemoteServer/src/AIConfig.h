#pragma once

#include <string>
#include "Properties.h"

class AIConfig
{
public:
	AIConfig(string configFile);
	~AIConfig();
	
	string Name;
	bool IsMute;

private:
	string GetNameFromProperty(Properties prop);
	bool GetMuteFromProperty(Properties prop);
};

