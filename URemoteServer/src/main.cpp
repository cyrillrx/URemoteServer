#include "AI.h"

#include <iostream>
#include <sstream>

#include "Utils.h"
#include "ServerConfig.h"
#include "Translator.h"
#include "fs_utils.h"
#include "exception\Exception.h"
#include "modules\Speech.h"
#include "modules\text_to_speech.h"

using namespace std;

bool initProgram(unique_ptr<AIConfig>& aiConfig, unique_ptr<ServerConfig>& serverConfig);
bool initAiConfig(unique_ptr<AIConfig>& aiConfig, string& message);
bool initTranslator(Translator* translator, string& message);
bool initServerConfig(unique_ptr<ServerConfig>& serverConfig, string& message);
string filenameToKey(const string& filename);

static const string language_dir	= "lang/";
static const string config_dir		= "conf/";
static const string ai_conf_path		= config_dir + "ai.conf";
static const string server_conf_path	= config_dir + "server.conf";
static const string log_path = "URemote.log";

logger* logger = Utils::getLogger();

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
		fs_utils::create_directory(logger::getLogDir());
		logger->debug("Log directory \"" + logger::getLogDir() + "\" have been created");
	} catch (const Exception& e) {
		logger->error(e.whatAsString());
	}

	logger->setLogFile(log_path);

	logger->info("******************************************************");
	logger->info("*****               URemote Server               *****");
	logger->info("******************************************************");

	unique_ptr<AIConfig> aiConfig = nullptr;
	unique_ptr<ServerConfig> serverConfig = nullptr;
	if (!initProgram(aiConfig, serverConfig)) {
		return EXIT_FAILURE;
	}

	// Create the AI
	auto artificialIntelligence = unique_ptr<AI>(new AI(move(aiConfig)));

	// Start the server on the AI
	artificialIntelligence->startConnection(move(serverConfig));

	Translator::freeInstance();
	return EXIT_SUCCESS;
}

/**
* Initialize the AI, the translator and the Server
*/
bool initProgram(unique_ptr<AIConfig>& aiConfig, unique_ptr<ServerConfig>& serverConfig)
{
	logger->info("Program initialization...");

	bool programInitialized = false;
	string message = "";

	// Init config for the AI
	bool aiInitialized = initAiConfig(aiConfig, message);

	// Init translator
	auto translator = Translator::getInstance();
	bool translatorInitialized = initTranslator(translator, message);
	if (translatorInitialized && aiInitialized) {
		translator->setLanguage(aiConfig->Lang);
	}

	bool serverInitialized = initServerConfig(serverConfig, message);

	programInitialized = aiInitialized && translatorInitialized && serverInitialized;

	// Check program initializations
	if (programInitialized) {
		logger->info("Program initialized.");
	} else {
		logger->error("Program initialization failed !");
		MessageBoxA(nullptr, message.c_str(), nullptr, 0);
	}

	return programInitialized;
}

/**
* Initialize the Translator.
* Load the language files stored in LANGUAGE_DIR and add them to the translator.
*/
bool initTranslator(Translator* translator, string& message) 
{
	logger->info("Init Translator...");

	auto files = fs_utils::list_files(language_dir, false, ".*(\\.lang)$", true);
	for (auto file : files) {

		try {
			translator->addLanguage(filenameToKey(file.filename()), file.path());
		} catch (const exception& e) {
			logger->warning(e.what());
			message += e.what();
			message += "\n";
		}
	}

	const auto isInitialized = translator->isInitialized();
	if (isInitialized) {
		logger->info("Translator OK.");
	} else {
		string errorMessage = "Translator KO : No language file available.";
		logger->error(errorMessage);
		message += errorMessage + "\n";
	}

	return isInitialized;
}

/**
* Get a filename and return the corresponding language key.
*/
string filenameToKey(const string& filename)
{
	const string defaultKey = Translator::LANG_EN;

	// TODO: Store the map in RAM ?
	map<string, string> langFilenameToKey;
	langFilenameToKey.insert(make_pair("fr.lang", Translator::LANG_FR));
	langFilenameToKey.insert(make_pair("en.lang", Translator::LANG_EN));

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
bool initAiConfig(unique_ptr<AIConfig>& aiConfig, string& message)
{
	logger->info("Init config for the AI...");
	bool aiInitialized = false;
	try {
		aiConfig = unique_ptr<AIConfig>(new AIConfig(ai_conf_path));
		logger->info("AI config OK.");
		text_to_speech::available_languages();
		aiInitialized = true;
	} catch (const exception& e) {
		message += e.what();
		message += "\n";
		logger->error("AI config KO.");
	}
	return aiInitialized;
}

/**
* Init config for the Server.
* Load the ServerConfig object with the properties found in SERVER_CONF_FILE 
*/
bool initServerConfig(unique_ptr<ServerConfig>& serverConfig, string& message)
{
	// Init config for the server
	logger->info("Init config for the server...");
	bool serverInitialized = false;
	try {
		serverConfig = unique_ptr<ServerConfig>(new ServerConfig(server_conf_path));
		logger->debug("Server config OK.");
		serverInitialized = true;
	} catch (const exception& e) {
		message += e.what();
		message += "\n";
		logger->error("Server config KO.");
	}
	return serverInitialized;
}