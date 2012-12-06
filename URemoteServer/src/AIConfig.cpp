#include "AIConfig.h"

using namespace std;

const string AIConfig::KEY_NAME		= "name";
const string AIConfig::KEY_LANG		= "lang";
const string AIConfig::KEY_GENDER	= "gender";
const string AIConfig::KEY_MUTE		= "mute";

const string AIConfig::DEFAULT_NAME		= "Eternity";
const string AIConfig::DEFAULT_LANG		= "EN";
const string AIConfig::DEFAULT_GENDER	= "F";
const bool AIConfig::DEFAULT_MUTE		= false;

AIConfig::AIConfig(const string& configFile) : Properties(configFile)
{
	Name	= GetString(KEY_NAME, DEFAULT_NAME);
	Lang	= GetString(KEY_LANG, DEFAULT_LANG);
	Gender	= GetString(KEY_GENDER, DEFAULT_GENDER);
	IsMute	= GetBool(KEY_MUTE, DEFAULT_MUTE);
}

AIConfig::~AIConfig()
{
	SetString(KEY_NAME, Name);
	SetString(KEY_LANG, Lang);
	SetString(KEY_GENDER, Gender);
	SetBool(KEY_MUTE, IsMute);

	SaveProperties();
}