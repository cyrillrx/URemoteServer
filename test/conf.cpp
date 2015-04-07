//
// Created by Cyril Leroux on 07/04/2015.
//

#include "lang/lexicon_manager.h"
#include "gtest/gtest.h"

const std::string CONF_AI_PATH = "../../blocks/cyrillrx/ubiquity/test/conf/ai.conf";
const std::string CONF_SERVER = "../../blocks/cyrillrx/ubiquity/test/conf/server.conf";
const std::string CONF_USER = "../../blocks/cyrillrx/ubiquity/test/conf/authorized_users.conf";
const std::string LANG_FR = "../../blocks/cyrillrx/ubiquity/test/lang/fr.lang";
const std::string LANG_EN = "../../blocks/cyrillrx/ubiquity/test/lang/en.lang";

/** @return the path of conf or lang directory */
std::string conf_dir(const std::string &filename)
{
    std::string currentDir = fs_utils::get_current_directory();
    return fs_utils::build_path(currentDir, filename);
}



TEST(Config, LoadAiConfig) {
bool aiInitialized = false;
std::unique_ptr<ai_config> &aiConfig
try {
aiConfig = std::unique_ptr<ai_config>(new ai_config(CONF_AI_PATH));

if (!text_to_speech::test_parameters(aiConfig->language_code(), aiConfig->gender, aiConfig->age, aiConfig->rate)) {

// Retry with default settings.
aiConfig->language = text_to_speech::default_lang;
if (!text_to_speech::test_parameters(aiConfig->language_code(), aiConfig->gender, aiConfig->age, aiConfig->rate)) {
FAIL() << "AiConfig : Try with default Failed";
}
}

aiInitialized = true;

} catch (const Exception &e) {
    FAIL() << "Ai Config KO : " + e.whatAsString());
}
SUCCESS();
}

TEST(Config, InitLexicon)
{
std::string path = fs_utils::get_current_directory();
auto lexiconMgr = lexicon_manager::instance();
lexiconMgr->add_language(lexicon_manager::LANG_EN_UK, language_dir + "en.lang");
lexiconMgr->add_language(lexicon_manager::LANG_EN_US, language_dir + "en.lang");
lexiconMgr->add_language(lexicon_manager::LANG_FR, language_dir + "fr.lang");

ASSERT_TRUE(lexiconMgr->is_initialized(), actualFilename) << "lexicon_manager KO : No language file available.";
}