#include "AIConfig.h"

const string AIConfig::KEY_NAME = "name";
const string AIConfig::KEY_MUTE = "mute";
const string AIConfig::DEFAULT_NAME = "Eternity";
const bool AIConfig::DEFAULT_MUTE = false;

AIConfig::AIConfig(const string& configFile) : Properties(configFile)
{
	Name = GetString(KEY_NAME, DEFAULT_NAME);
	IsMute = GetBool(KEY_MUTE, DEFAULT_MUTE);
}

AIConfig::~AIConfig()
{
	SetString(KEY_NAME, Name);
	SetBool(KEY_MUTE, IsMute);
	SaveProperties();
}