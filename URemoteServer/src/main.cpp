#include "AI.h"

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <winuser.h>

#include "../../libraries/lrx_cross_api/src/network_io/server_config.h"
#include "../../libraries/lrx_core/Utils/src/logger.h"
#include "../../libraries/lrx_core/Utils/src/Utils.h"
#include "../../libraries/lrx_core/Utils/src/exception/Exception.h"
#include "../../libraries/lrx_core/Utils/src/lexicon_manager.h"
#include "../../libraries/lrx_core/Utils/src/fs_utils.h"
#include "../../libraries/lrx_cross_api/src/text_to_speech.h"
#include "../../libraries/lrx_core/Utils/src/exception/config_exception.h"

void createDirectories();
bool initProgram(std::unique_ptr<ai_config>& aiConfig,
				 std::unique_ptr<authorized_users>& users,
				 std::unique_ptr<network_io::server_config>& serverConfig);
bool loadAiConfig(std::unique_ptr<ai_config>& aiConfig, std::string& message);
bool initLexicons(lexicon_manager* lexiconMgr, std::string& message);
bool loadServerConfig(std::unique_ptr<network_io::server_config>& server_config, std::string& message);
bool loadUsers(std::unique_ptr<authorized_users>& users_config, std::string& message);
std::string filenameToKey(const std::string& filename);

static const std::string language_dir	= "lang/";
static const std::string config_dir		= "conf/";
static const std::string ai_conf_path		= config_dir + "ai.conf";
static const std::string server_conf_path	= config_dir + "server.conf";
static const std::string users_conf_path	= config_dir + "authorized_users.conf";
static const std::string log_path = "URemote.log";

logger* logger = Utils::get_logger();

int main()
{
	logger->info("URemote Server starting");
	createDirectories();

	logger->set_log_file(log_path);


	std::unique_ptr<ai_config> aiConfig		= nullptr;
	std::unique_ptr<authorized_users> users	= nullptr;
	std::unique_ptr<network_io::server_config> serverConfig = nullptr;

	const auto isInitialized = initProgram(aiConfig, users, serverConfig);
	if (!isInitialized) {
		logger->error("Program not initialized. Leaving URemote Server : EXIT_FAILURE.");
		return EXIT_FAILURE;
	}

	// Create the AI
	auto artificialIntelligence = std::make_shared<AI>(std::move(aiConfig), std::move(users));

	// Start the server on the AI
	artificialIntelligence->startConnection(std::move(serverConfig));

	lexicon_manager::free_instance();

	logger->info("Leaving URemote Server : EXIT_SUCCESS");
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
	logger->debug("Directories initialization...");

	try {
		fs_utils::create_directory(language_dir);
		logger->debug("Language directory \"" + language_dir + "\" have been created.");
	} catch (const Exception& e) {
		logger->error(e.whatAsString());
	}

	try {
		fs_utils::create_directory(config_dir);
		logger->debug("Configuration directory \"" + config_dir + "\" have been created");
	} catch (const Exception& e) {
		logger->error(e.whatAsString());
	}

	try {
		fs_utils::create_directory(logger::log_dir());
		logger->debug("Log directory \"" + logger::log_dir() + "\" have been created");
	} catch (const Exception& e) {
		logger->error(e.whatAsString());
	}

	logger->debug("Directory created.");
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
	logger->info("Program initialization...");
	// TODO: Set Error and warning into a Queue to treat it (vocally) once everything is loaded.

	bool programInitialized = false;
	std::string message = "";

	// Init config for the AI
	bool aiInitialized = loadAiConfig(aiConfig, message);

	// Init lexicon_manager
	auto lexiconMgr = lexicon_manager::instance();
	bool translatorInitialized = initLexicons(lexiconMgr, message);
	if (translatorInitialized && aiInitialized) {
		try {
			lexiconMgr->set_language(aiConfig->language);
		} catch (const Exception& e) {
			message += e.whatAsString();
			message += "\n";
			logger->error("AI config KO.");
		}
	}

	bool usersLoaded = loadUsers(users, message);
	bool serverInitialized = loadServerConfig(serverConfig, message);

	programInitialized = aiInitialized && translatorInitialized && usersLoaded && serverInitialized;

	// Check program initializations
	if (programInitialized) {
		logger->info("Program initialized.");
	} else {
		logger->error("Program initialization failed !");
# if defined(WINDOWS_PLATFORM)
		MessageBoxA(nullptr, message.c_str(), nullptr, 0);
# else
		// TODO: Implement messagebox with Qt or another widget library
# endif
	}

	return programInitialized;
}

/**
* Initialize the lexicon_manager.
* Load the language files stored in LANGUAGE_DIR and add them to the lexicon_manager.
*/
bool initLexicons(lexicon_manager* lexiconMgr, std::string& message)
{
	logger->info("Init lexicon_manager...");

	lexiconMgr->add_language(lexicon_manager::LANG_EN_UK, language_dir + "en.lang");
	lexiconMgr->add_language(lexicon_manager::LANG_EN_US, language_dir + "en.lang");
	lexiconMgr->add_language(lexicon_manager::LANG_FR, language_dir + "fr.lang");

	const auto isInitialized = lexiconMgr->is_initialized();
	if (isInitialized) {
		logger->info("lexicon_manager OK.");
	} else {
		std::string errorMessage = "lexicon_manager KO : No language file available.";
		logger->error(errorMessage);
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
	logger->debug("langFilenameToKey, file \"" + filename + "\" is not supported.");
	return defaultKey;
}

/**
* Load Artificial Intelligence configurations from a property file.
*/
bool loadAiConfig(std::unique_ptr<ai_config>& aiConfig, std::string& message)
{
	logger->info("Init config for the AI...");
	bool aiInitialized = false;
	try {
		aiConfig = std::unique_ptr<ai_config>(new ai_config(ai_conf_path));
		logger->info("AI config OK.");

		if (!text_to_speech::test_parameters(aiConfig->language_code(), aiConfig->gender, aiConfig->age, aiConfig->rate)) {
			message += "AI setting failure. Trying out with default settings\n";
			logger->warning("AI setting failure. Trying out with default settings");

			// Retry with default settings.
			aiConfig->language = text_to_speech::default_lang;
			if (!text_to_speech::test_parameters(aiConfig->language_code(), aiConfig->gender, aiConfig->age, aiConfig->rate)) {
				throw config_exception("main.cpp initAiConfig()", "AiConfig : Try with default Failed");
			}
		}

		aiInitialized = true;

	} catch (const Exception& e) {
		message += e.whatAsString();
		message += "\n";
		logger->error("AI config KO : " + e.whatAsString());
	}
	return aiInitialized;
}

/**
* Load URemoteListener configuration from a property file.
*/
bool loadServerConfig(std::unique_ptr<network_io::server_config>& server_config, std::string& message)
{
	// Init config for the server
	logger->info("Init config for the server...");
	bool serverInitialized = false;
	try {
		server_config = std::unique_ptr<network_io::server_config>(new network_io::server_config(server_conf_path));
		std::stringstream ss;
		ss << "Server config OK on port " << server_config->port();
		logger->debug(ss.str());
		serverInitialized = true;
	} catch (const Exception& e) {
		message += e.whatAsString();
		message += "\n";
		logger->error("Server config KO.");
	}
	return serverInitialized;
}

/**
* Load the authorized users from users_conf_path.
*/
bool loadUsers(std::unique_ptr<authorized_users>& users_config, std::string& message)
{
	// Loading authorized users
	logger->info("Loading authorized users...");
	bool usersLoaded = false;
	try {
		users_config = std::unique_ptr<authorized_users>(new authorized_users(users_conf_path));
		logger->debug("Users loaded.");
		usersLoaded = true;

	} catch (const Exception& e) {
		message += e.whatAsString();
		message += "\n";
		logger->error("Users Loading KO.");
	}
	return usersLoaded;
}
