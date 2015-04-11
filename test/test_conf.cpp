//
// Created by Cyril Leroux on 07/04/2015.
//

#include <memory>
#include <fs/fs_utils.h>
#include <exception/Exception.h>

#include "../src/ai_config.h"
#include "lang/lexicon_manager.h"
#include "gtest/gtest.h"

const std::string currentDir = fs_utils::get_current_directory();

const std::string CONF_AI_PATH = fs_utils::build_path(currentDir, "../../blocks/cyrillrx/ubiquity/test/assets/ai.conf");
const std::string CONF_SERVER = fs_utils::build_path(currentDir, "../../blocks/cyrillrx/ubiquity/test/assets/server.conf");
const std::string CONF_USER = fs_utils::build_path(currentDir, "../../blocks/cyrillrx/ubiquity/test/assets/authorized_users.conf");
const std::string LANG_FR = fs_utils::build_path(currentDir, "../../blocks/cyrillrx/ubiquity/test/assets/fr.lang");
const std::string LANG_EN = fs_utils::build_path(currentDir, "../../blocks/cyrillrx/ubiquity/test/assets/en.lang");

TEST(Config, LoadAiConfig)
{
    std::unique_ptr<ai_config> aiConfig;
    try {
        aiConfig = std::unique_ptr<ai_config>(new ai_config(CONF_AI_PATH));

        ASSERT_EQ(aiConfig->name, "Jarvis") << "AI name is not as expected";
        ASSERT_EQ(aiConfig->language, "EN") << "AI language is not as expected";
        ASSERT_EQ(aiConfig->gender, "F") << "AI gender is not as expected";
        ASSERT_EQ(aiConfig->is_mute, false) << "AI mute state is not as expected";

    } catch (const Exception &e) {
        FAIL() << "Ai Config KO : " << e.whatAsString();
    }
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