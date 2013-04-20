#include "AI.h"

#include <iostream>
#include <sstream>

#include "platform_config.h"
#include "Utils.h"
#include "lexicon_manager.h"
#include "fs_utils.h"
#include "network_io/server_config.h"
#include "exception/config_exception.h"
#include "modules/Speech.h"
#include "text_to_speech.h"
#include "logger.h"

bool initProgram(std::unique_ptr<ai_config>& aiConfig, std::unique_ptr<network_io::server_config>& server_config);
bool initAiConfig(std::unique_ptr<ai_config>& aiConfig, std::string& message);
bool initLexicons(lexicon_manager* lexiconMgr, std::string& message);
bool initServerConfig(std::unique_ptr<network_io::server_config>& server_config, std::string& message);
std::string filenameToKey(const std::string& filename);

static const std::string language_dir	= "lang/";
static const std::string config_dir		= "conf/";
static const std::string ai_conf_path		= config_dir + "ai.conf";
static const std::string server_conf_path	= config_dir + "server.conf";
static const std::string log_path = "URemote.log";

logger* logger = Utils::get_logger();

int main()
{
	logger->debug("******************************************************");
	logger->debug("*****          Directory initialization          *****");
	logger->debug("******************************************************");

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

	logger->set_log_file(log_path);

	logger->info("******************************************************");
	logger->info("*****               URemote Server               *****");
	logger->info("******************************************************");

	std::unique_ptr<ai_config> aiConfig = nullptr;
	std::unique_ptr<network_io::server_config> serverConfig = nullptr;
	if (!initProgram(aiConfig, serverConfig)) {
		logger->info("******************************************************");
		logger->info("*****   Leaving URemote Server : EXIT_FAILURE    *****");
		logger->info("******************************************************");
		return EXIT_FAILURE;
	}

	// Create the AI
	auto artificialIntelligence = std::make_shared<AI>(std::move(aiConfig));

	// Start the server on the AI
	artificialIntelligence->startConnection(std::move(serverConfig));

	lexicon_manager::free_instance();

	logger->info("******************************************************");
	logger->info("*****   Leaving URemote Server : EXIT_SUCCESS    *****");
	logger->info("******************************************************");
	return EXIT_SUCCESS;
}

/**
* Initialize the AI, the lexicon_manager and the Server
*/
bool initProgram(std::unique_ptr<ai_config>& aiConfig, std::unique_ptr<network_io::server_config>& serverConfig)
{
	logger->info("Program initialization...");
	// TODO: Set Error and warning into a Queue to treat it (vocally) once everything is loaded.

	bool programInitialized = false;
	std::string message = "";

	// Init config for the AI
	bool aiInitialized = initAiConfig(aiConfig, message);

	// Init lexicon_manager
	auto lexiconMgr = lexicon_manager::instance();
	bool translatorInitialized = initLexicons(lexiconMgr, message);
	if (translatorInitialized && aiInitialized) {
		lexiconMgr->set_language(aiConfig->language);
	}

	bool serverInitialized = initServerConfig(serverConfig, message);

	programInitialized = aiInitialized && translatorInitialized && serverInitialized;

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

	auto files = fs_utils::list_files(language_dir, false, ".*(\\.lang)$", true);
	for (auto file : files) {

		try {
			lexiconMgr->add_language(filenameToKey(file.filename()), file.path());
		} catch (const Exception& e) {
			logger->warning(e.whatAsString());
			message += e.whatAsString();
			message += "\n";
		}
	}

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
	const auto defaultKey = lexicon_manager::LANG_EN;

	// TODO: Store the map in RAM ?
	std::map<std::string, std::string> langFilenameToKey;
	langFilenameToKey.insert(std::make_pair("fr.lang", lexicon_manager::LANG_FR));
	langFilenameToKey.insert(std::make_pair("en.lang", lexicon_manager::LANG_EN));

	auto keyItem = langFilenameToKey.find(filename);
	if (keyItem != langFilenameToKey.end()) {
		return keyItem->second;
	}

	// If key is not found, return the default key
	logger->debug("langFilenameToKey, file \"" + filename + "\" is not supported.");
	return defaultKey;
}

/**
* Init config for the AI
*/
bool initAiConfig(std::unique_ptr<ai_config>& aiConfig, std::string& message)
{
	logger->info("Init config for the AI...");
	bool aiInitialized = false;
	try {
		aiConfig = std::unique_ptr<ai_config>(new ai_config(ai_conf_path));
		logger->info("AI config OK.");

		if (!text_to_speech::test_parameters(aiConfig->language, aiConfig->gender)) {
			message += "AI setting failure. Trying out with default settings\n";
			logger->warning("AI setting failure. Trying out with default settings");

			// Retry with default settings.
			aiConfig->language = text_to_speech::default_lang;
			if (!text_to_speech::test_parameters(aiConfig->language, aiConfig->gender)) {
				throw config_exception("main.cpp initAiConfig()", "AiConfig : Try with default Failed");
			}
		}

		aiInitialized = true;

	} catch (const Exception& e) {
		message += e.whatAsString();
		message += "\n";
		logger->error("AI config KO.");
	}
	return aiInitialized;
}

/**
* Init config for the Server.
* Load the server_config object with the properties found in SERVER_CONF_FILE
*/
bool initServerConfig(std::unique_ptr<network_io::server_config>& server_config, std::string& message)
{
	// Init config for the server
	logger->info("Init config for the server...");
	bool serverInitialized = false;
	try {
		server_config = std::unique_ptr<network_io::server_config>(new network_io::server_config(server_conf_path));
		logger->debug("Server config OK.");
		serverInitialized = true;
	} catch (const Exception& e) {
		message += e.whatAsString();
		message += "\n";
		logger->error("Server config KO.");
	}
	return serverInitialized;
}
