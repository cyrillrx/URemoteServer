#include "AI.h"

#include <iostream>
#include <sstream>

# if defined(WINDOWS_PLATFORM)
#   include <winuser.h>
# endif

#include "logger/logger.h"
#include "logger/logger_manager.h"
#include "logger/console_logger.h"
#include "logger/file_logger.h"
// TODO update Biicode config
//#include "text_to_speech.h"
//#include "network_io/server_config.h"
#include "cyrillrx/cross_api/src/text_to_speech.h"

#include "modules/file_manager.h"
#include "exception/Exception.h"
#include "exception/config_exception.h"
#include "lang/lexicon_manager.h"
#include "fs/fs_utils.h"

void createDirectories();

bool initProgram(std::unique_ptr<ai_config>& aiConfig,
                 std::unique_ptr<authorized_users>& users,
                 std::unique_ptr<network_io::server_config>& serverConfig);

bool loadAiConfig(std::unique_ptr<ai_config>& aiConfig, std::string& message);

bool initTextToSpeech(const std::unique_ptr<ai_config>& aiConfig, std::string& message);

bool initLexicons(lexicon_manager *lexiconMgr, std::string& message);

bool loadServerConfig(std::unique_ptr<network_io::server_config>& server_config, std::string& message);

bool loadUsers(std::unique_ptr<authorized_users>& users_config, std::string& message);

std::string filenameToKey(const std::string& filename);

static const std::string log_dir = "logs/";
static const std::string language_dir = "lang/";
static const std::string config_dir = "conf/";
static const std::string ai_conf_path = config_dir + "ai.conf";
static const std::string server_conf_path = config_dir + "server.conf";
static const std::string users_conf_path = config_dir + "users.conf";
static const std::string log_path = log_dir + "uremote.log";

auto logManager = LoggerManager();

int main()
{
    std::unique_ptr<Logger> consoleLogger(new ConsoleLogger(DEBUG));
    logManager.AddLogger(consoleLogger);

    logManager.Info("-----------------------------------");
    logManager.Info("URemote Server starting");
    logManager.Info("-----");
    createDirectories();

    std::unique_ptr<Logger> fileLogger(new FileLogger(log_path, DEBUG));
    logManager.AddLogger(fileLogger);

    std::unique_ptr<ai_config> aiConfig = nullptr;
    std::unique_ptr<authorized_users> users = nullptr;
    std::unique_ptr<network_io::server_config> serverConfig = nullptr;

    const auto isInitialized = initProgram(aiConfig, users, serverConfig);
    if (!isInitialized) {
        logManager.Error("Program not initialized. Leaving URemote Server : EXIT_FAILURE.");
        return EXIT_FAILURE;
    }

    // Create the AI
    auto artificialIntelligence = std::make_shared<AI>(std::move(aiConfig), std::move(users));

    // Start the server on the AI
    artificialIntelligence->startConnection(std::move(serverConfig));

    lexicon_manager::free_instance();

    logManager.Info("Leaving URemote Server : EXIT_SUCCESS");
    return EXIT_SUCCESS;
}

/**
 * Creates the required directories :
 * - language_dir
 * - config_dir
 * - log_dir
 */
void createDirectories()
{
    logManager.Debug("-----------------------------------");
    logManager.Debug("Check directories initialization...");
    logManager.Debug("-----");

    if (!fs_utils::exists(language_dir)) {
        try {
            fs_utils::create_directory(language_dir);
            logManager.Debug("Language directory \"" + language_dir + "\" have been created.");
        } catch (const Exception& e) {
            logManager.Error(e.whatAsString());
        }
    }

    if (!fs_utils::exists(config_dir)) {
        try {
            fs_utils::create_directory(config_dir);
            logManager.Debug("Configuration directory \"" + config_dir + "\" have been created");
        } catch (const Exception& e) {
            logManager.Error(e.whatAsString());
        }
    }

    if (!fs_utils::exists(log_dir)) {
        try {
            fs_utils::create_directory(log_dir);
            logManager.Debug("Log directory \"" + log_dir + "\" have been created");
        } catch (const Exception& e) {
            logManager.Error(e.whatAsString());
        }
    }

    logManager.Debug("Directory created.");
}

/**
 * Initialize the AI, the lexicon_manager and the Server
 * Load the program configurations :
 * - Ai config
 * - Authorized Users
 * - Server (URemoteListener) configuration
 */
bool initProgram(std::unique_ptr<ai_config>& aiConfig,
                 std::unique_ptr<authorized_users>& users,
                 std::unique_ptr<network_io::server_config>& serverConfig)
{
    logManager.Info("-----------------------------------");
    logManager.Info("Program initialization...");
    logManager.Info("-----------------------------------");
    // TODO: Set Error and warning into a Queue to treat it (vocally) once everything is loaded.

    bool programInitialized;
    std::string message = "";

    // Init config for the AI
    bool aiInitialized = loadAiConfig(aiConfig, message);
    initTextToSpeech(aiConfig, message);

    // Init lexicon_manager
    auto lexiconMgr = lexicon_manager::instance();
    bool translatorInitialized = initLexicons(lexiconMgr, message);
    if (translatorInitialized && aiInitialized) {
        try {
            lexiconMgr->set_language(aiConfig->language);
        } catch (const Exception& e) {
            message += e.whatAsString();
            message += "\n";
            logManager.Error("lexiconMgr->set_language failed");
        }
    }

    bool usersLoaded = loadUsers(users, message);
    bool serverInitialized = loadServerConfig(serverConfig, message);

    programInitialized = aiInitialized && translatorInitialized && usersLoaded && serverInitialized;

    // Check program initializations
    if (programInitialized) {
        logManager.Info("Program initialized.");
    } else {
        logManager.Error("Program initialization failed !");
# if defined(WINDOWS_PLATFORM)
        MessageBoxA(nullptr, message.c_str(), nullptr, 0);
# else
        // TODO: Implement messagebox with Qt or another widget library
        logManager.Error("An error occured:" + message);
# endif
    }

    return programInitialized;
}

/**
 * Initialize the lexicon_manager.
 * Load the language files stored in LANGUAGE_DIR and add them to the lexicon_manager.
 */
bool initLexicons(lexicon_manager *lexiconMgr, std::string& message)
{
    logManager.Info("-----------------------------------");
    logManager.Info("Init lexicon_manager...");
    logManager.Info("-----");

    lexiconMgr->add_language(lexicon_manager::LANG_EN_UK, language_dir + "en.lang");
    lexiconMgr->add_language(lexicon_manager::LANG_EN_US, language_dir + "en.lang");
    lexiconMgr->add_language(lexicon_manager::LANG_FR, language_dir + "fr.lang");

    const auto isInitialized = lexiconMgr->is_initialized();
    if (isInitialized) {
        logManager.Info("lexicon_manager OK.");
    } else {
        std::string errorMessage = "lexicon_manager KO : No language file available.";
        logManager.Error(errorMessage);
        message += errorMessage + "\n";
    }

    return isInitialized;
}

/**
 * Get a filename and return the corresponding language key.
 */
std::string filenameToKey(const std::string& filename)
{
    const auto defaultKey = lexicon_manager::LANG_EN_US;

    // TODO: Store the map in RAM ?
    std::map<std::string, std::string> langFilenameToKey;
    langFilenameToKey.insert(std::make_pair("fr.lang", lexicon_manager::LANG_FR));
    langFilenameToKey.insert(std::make_pair("en.lang", lexicon_manager::LANG_EN_US));
    langFilenameToKey.insert(std::make_pair("en.lang", lexicon_manager::LANG_EN_UK));

    auto keyItem = langFilenameToKey.find(filename);
    if (keyItem != langFilenameToKey.end()) {
        return keyItem->second;
    }

    // If key is not found, return the default key
    logManager.Debug("langFilenameToKey, file \"" + filename + "\" is not supported.");
    return defaultKey;
}

/**
 * Load Artificial Intelligence configurations from a property file.
 */
bool loadAiConfig(std::unique_ptr<ai_config>& aiConfig, std::string& message)
{
    logManager.Info("-----------------------------------");
    logManager.Info("Init config for the AI...");
    logManager.Info("-----");
    try {
        aiConfig = std::unique_ptr<ai_config>(new ai_config(ai_conf_path));
        logManager.Info("AI config OK.");
        return true;

    } catch (const Exception& e) {
        message += e.whatAsString();
        message += "\n";
        logManager.Error("AI config KO : " + e.whatAsString());
        return false;
    }
}

bool initTextToSpeech(const std::unique_ptr<ai_config>& aiConfig, std::string& message)
{
    if (!text_to_speech::is_implemented()) {
        logManager.Info("-----------------------------------");
        logManager.Info("Text-to-speech not implemented.");
        return false;
    }

    logManager.Info("-----------------------------------");
    logManager.Info("Initializing Text-to-speech...");
    logManager.Info("-----");
    try {

        bool ttsSettingOK = text_to_speech::test_parameters(
                aiConfig->language_code(),
                aiConfig->gender,
                aiConfig->age,
                aiConfig->rate);

        if (ttsSettingOK) {
            logManager.Info("Text-to-speech config OK.");
            return true;

        } else {
            message += "Text-to-speech setting failure. Trying out with default settings\n";
            logManager.Warning("Text-to-speech setting failure. Trying out with default settings");
        }

        // Retry with default settings.
        aiConfig->language = text_to_speech::default_lang;

        bool ttsDefaultSettingOK = text_to_speech::test_parameters(
                aiConfig->language_code(),
                aiConfig->gender,
                aiConfig->age,
                aiConfig->rate);

        if (!ttsDefaultSettingOK) {
            throw config_exception("initTextToSpeech()", "Text-to-speech : Try with default Failed");
        }

    } catch (const Exception& e) {
        message += e.whatAsString();
        message += "\n";
        logManager.Error("Text-to-speech config KO : " + e.whatAsString());
        return false;
    }
    return true;
}

/**
 * Load URemoteListener configuration from a property file.
 */
bool loadServerConfig(std::unique_ptr<network_io::server_config>& server_config, std::string& message)
{
    // Init config for the server
    logManager.Info("-----------------------------------");
    logManager.Info("Init config for the server...");
    logManager.Info("-----");
    bool serverInitialized = false;
    try {
        server_config = std::unique_ptr<network_io::server_config>(new network_io::server_config(server_conf_path));
        file_manager::set_default_path(server_config->root());
        std::stringstream ss;
        ss << "Server config OK on port " << server_config->port();
        logManager.Debug(ss.str());
        serverInitialized = true;
    } catch (const Exception& e) {
        message += e.whatAsString();
        message += "\n";
        logManager.Error("Server config KO : " + e.whatAsString());
    }
    return serverInitialized;
}

/**
 * Load the authorized users from users_conf_path.
 */
bool loadUsers(std::unique_ptr<authorized_users>& users_config, std::string& message)
{
    // Loading authorized users
    logManager.Info("-----------------------------------");
    logManager.Info("Loading authorized users...");
    logManager.Info("-----");
    bool usersLoaded = false;
    try {
        users_config = std::unique_ptr<authorized_users>(new authorized_users(users_conf_path));
        logManager.Debug("Users loaded.");
        usersLoaded = true;

    } catch (const Exception& e) {
        message += e.whatAsString();
        message += "\n";
        logManager.Error("Users Loading KO : " + e.whatAsString());
    }
    return usersLoaded;
}
