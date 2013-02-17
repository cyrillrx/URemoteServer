#include "AIConfig.h"

const std::string AIConfig::KEY_NAME	= "name";
const std::string AIConfig::KEY_LANG	= "lang";
const std::string AIConfig::KEY_GENDER	= "gender";
const std::string AIConfig::KEY_MUTE	= "mute";

const std::string AIConfig::DEFAULT_NAME	= "Eternity";
const std::string AIConfig::DEFAULT_LANG	= "EN";
const std::string AIConfig::DEFAULT_GENDER	= "F";
const bool AIConfig::DEFAULT_MUTE		= false;

AIConfig::AIConfig(const std::string& configFile) : Properties(configFile)
{
	Name	= getString(KEY_NAME, DEFAULT_NAME);
	Lang	= getString(KEY_LANG, DEFAULT_LANG);
	Gender	= getString(KEY_GENDER, DEFAULT_GENDER);
	IsMute	= getBool(KEY_MUTE, DEFAULT_MUTE);
}

AIConfig::~AIConfig()
{
	setString(KEY_NAME, Name);
	setString(KEY_LANG, Lang);
	setString(KEY_GENDER, Gender);
	setBool(KEY_MUTE, IsMute);

	saveProperties();
}