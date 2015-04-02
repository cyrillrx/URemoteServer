#pragma once

#include <string>
#include "voice.h"
#include "properties.h"

class ai_config : public properties
{
public:
    ai_config(const std::string &path);

    ~ai_config();

    const text_to_speech::language_code language_code();

    const text_to_speech::tts_age str_to_age(const std::string &age);

    const std::string age_to_str(const text_to_speech::tts_age &age);

    std::string name;
    std::string language;
    std::string gender;
    text_to_speech::tts_age age;
    std::string tts_name;
    bool is_mute;
    long rate;


    static const std::string DEFAULT_LANG;

private:
    static const std::string KEY_NAME;
    static const std::string KEY_LANG;
    static const std::string KEY_GENDER;
    static const std::string KEY_AGE;
    static const std::string KEY_TTS_NAME;
    static const std::string KEY_MUTE;
    static const std::string KEY_RATE;

    static const std::string DEFAULT_NAME;
    static const std::string DEFAULT_GENDER;
    static const std::string DEFAULT_AGE;
    static const std::string DEFAULT_TTS_NAME;
    static const bool DEFAULT_MUTE;
    static const long DEFAULT_RATE;
};