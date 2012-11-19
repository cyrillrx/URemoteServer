#include "AIConfig.h"

AIConfig::AIConfig(string configFile)
{
	Properties properties;
	properties.LoadProperties(configFile);
	
	Name = GetNameFromProperty(properties);
	IsMute = GetMuteFromProperty(properties);
}

AIConfig::~AIConfig()
{
}

string AIConfig::GetNameFromProperty(Properties prop)
{
	try {
		return prop.GetString("name");
	} catch (ReadPropertyException const& exception) {
		return "Eternity";
	}
}

bool AIConfig::GetMuteFromProperty(Properties prop)
{
	try {
		return prop.GetBool("mute");
	} catch (ReadPropertyException const& exception) {
		return false;
	}
}