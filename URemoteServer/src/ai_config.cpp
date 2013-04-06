#include "ai_config.h"

const std::string ai_config::KEY_NAME	= "name";
const std::string ai_config::KEY_LANG	= "lang";
const std::string ai_config::KEY_GENDER	= "gender";
const std::string ai_config::KEY_MUTE	= "mute";

const std::string ai_config::DEFAULT_NAME	= "Eternity";
const std::string ai_config::DEFAULT_LANG	= "EN";
const std::string ai_config::DEFAULT_GENDER	= "F";
const bool ai_config::DEFAULT_MUTE			= false;

ai_config::ai_config(const std::string& configFile) : properties(configFile)
{
	name		= get_string(KEY_NAME, DEFAULT_NAME);
	language	= get_string(KEY_LANG, DEFAULT_LANG);
	gender		= get_string(KEY_GENDER, DEFAULT_GENDER);
	is_mute		= get_bool(KEY_MUTE, DEFAULT_MUTE);
}

ai_config::~ai_config()
{
	set_string(KEY_NAME, name);
	set_string(KEY_LANG, language);
	set_string(KEY_GENDER, gender);
	set_bool(KEY_MUTE, is_mute);

	save_properties();
}