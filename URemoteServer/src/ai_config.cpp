#include "ai_config.h"

const std::string ai_config::KEY_NAME		= "name";
const std::string ai_config::KEY_LANG		= "lang";
const std::string ai_config::KEY_GENDER		= "gender";
const std::string ai_config::KEY_AGE		= "age";
const std::string ai_config::KEY_TTS_NAME	= "tts_name";
const std::string ai_config::KEY_MUTE		= "mute";
const std::string ai_config::KEY_RATE		= "rate";

const std::string ai_config::DEFAULT_NAME		= "Eternity";
const std::string ai_config::DEFAULT_LANG		= "UK";
const std::string ai_config::DEFAULT_GENDER		= "F";
const std::string ai_config::DEFAULT_AGE		= "Adult";
const std::string ai_config::DEFAULT_TTS_NAME	= "Amy";
const bool ai_config::DEFAULT_MUTE				= false;
const long ai_config::DEFAULT_RATE				= 0;

/**
 * Possible values for "language" :	"FR", "US" or "UK".
 * Possible values for "gender" :	"Male" or "Female".
 * Possible values for "age" :		"Child", "Teen", "Adult" or "Senior".
 */
ai_config::ai_config(const std::string& configFile) : properties(configFile)
{
	name		= get_string(KEY_NAME, DEFAULT_NAME);
	language	= get_string(KEY_LANG, DEFAULT_LANG);
	gender		= get_string(KEY_GENDER, DEFAULT_GENDER);
	age			= str_to_age(get_string(KEY_AGE, DEFAULT_GENDER));
	tts_name	= get_string(KEY_TTS_NAME, DEFAULT_TTS_NAME);
	is_mute		= get_bool(KEY_MUTE, DEFAULT_MUTE);
	rate		= get_long(KEY_RATE, DEFAULT_RATE);
}

ai_config::~ai_config()
{
	set_string(KEY_NAME, name);
	set_string(KEY_LANG, language);
	set_string(KEY_GENDER, gender);
	set_string(KEY_AGE, age_to_str(age));
	set_string(KEY_TTS_NAME, tts_name);
	set_bool(KEY_MUTE, is_mute);
	set_long(KEY_MUTE, is_mute);

	save_properties();
}

const text_to_speech::language_code ai_config::language_code()
{
	if (language == "UK") {
		return text_to_speech::en_UK;

	} else if (language == "FR") {
		return text_to_speech::fr_FR;
	}

	return text_to_speech::en_US;
}

const text_to_speech::tts_age ai_config::str_to_age(const std::string& age)
{
	if (age == "Child") {
		return text_to_speech::Child;

	} else if (age == "Teen") {
		return text_to_speech::Teen;

	} else if (age == "Senior") {
		return text_to_speech::Senior;
	}

	return text_to_speech::Adult;
}

const std::string ai_config::age_to_str(const text_to_speech::tts_age& age)
{
	if (age == text_to_speech::Child) {
		return "Child";

	} else if (age == text_to_speech::Teen) {
		return "Teen";

	} else if (age == text_to_speech::Senior) {
		return "Senior";
	}

	return "Adult";
}