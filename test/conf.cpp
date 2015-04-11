//
// Created by Cyril Leroux on 07/04/2015.
//

#include <memory>
#include <fs/fs_utils.h>
#include <exception/Exception.h>

#include "../src/ai_config.h"
#include "lang/lexicon_manager.h"
#include <cyrillrx/cross_api/src/text_to_speech.h>
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


TEST(Config, LoadAiConfig)
{
    std::unique_ptr<ai_config> aiConfig;
    try {
        aiConfig = std::unique_ptr<ai_config>(new ai_config(CONF_AI_PATH));

        if (!text_to_speech::test_parameters(
                aiConfig->language_code(),
                aiConfig->gender,
                aiConfig->age,
                aiConfig->rate)) {

// Retry with default settings.
            aiConfig->language = text_to_speech::default_lang;
            if (!text_to_speech::test_parameters(
                    aiConfig->language_code(),
                    aiConfig->gender,
                    aiConfig->age,
                    aiConfig->rate)) {
                FAIL() << "AiConfig : Try with default Failed";
            }
        }

    } catch (const Exception &e) {
        FAIL() << "Ai Config KO : " << e.whatAsString();
    }
    SUCCEED();
}

TEST(Config, InitLexicon)
{
    auto lexiconMgr = lexicon_manager::instance();
    lexiconMgr->add_language(lexicon_manager::LANG_EN_UK, LANG_EN);
    lexiconMgr->add_language(lexicon_manager::LANG_EN_US, LANG_EN);
    lexiconMgr->add_language(lexicon_manager::LANG_FR, LANG_FR);

    ASSERT_TRUE(lexiconMgr->is_initialized()) << "lexicon_manager KO : No language file available.";
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}