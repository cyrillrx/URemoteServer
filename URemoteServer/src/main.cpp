#include "AI.h"
#include "ServerConfig.h"
#include "Translator.h"
#include "Logger.h"
#include <Windows.h>
#include <iostream>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

bool initProgram(unique_ptr<AIConfig> aiConfig, unique_ptr<ServerConfig> serverConfig);
bool initTranslator(Translator* translator, string& message);

Logger logger("URemote.log");

int main()
{
	unique_ptr<AIConfig> aiConfig = nullptr;
	unique_ptr<ServerConfig> serverConfig = nullptr;
	if (!initProgram(move(aiConfig), move(serverConfig))) {
		return EXIT_FAILURE;
	}
	
	// Create the AI
	auto artificialIntelligence = unique_ptr<AI>(new AI(move(aiConfig)));
	
	// Start the server on the AI
	artificialIntelligence->StartConnection(move(serverConfig));
	
	Translator::freeInstance();
    return EXIT_SUCCESS;
}

bool initProgram(unique_ptr<AIConfig> aiConfig, unique_ptr<ServerConfig> serverConfig)
{
	logger.debug("Program initialization...");

	bool programInitialized = true;
	string message = "";

	// TODO: Init logger
	
	// Init config for the AI
	bool aiInitialized = false;
	try {
		aiConfig = unique_ptr<AIConfig>(new AIConfig("ai.conf"));
		aiInitialized = true;
	} catch (const exception& e) {
		message += e.what();
		message += "\n";
	}
	programInitialized &= aiInitialized;

	// Init translator
	auto translator = Translator::getInstance();
	bool translatorInitialized = initTranslator(translator, message);
	if (translatorInitialized && aiInitialized) {
		translator->setLanguage(aiConfig->Lang);
	}
	programInitialized &= translatorInitialized;

	// Init config for the server
	bool serverInitialized = false;
	try {
		serverConfig = unique_ptr<ServerConfig>(new ServerConfig("server.conf"));
		serverInitialized = true;
	} catch (const exception& e) {
		message += e.what();
		message += "\n";
	}
	programInitialized &= serverInitialized;

	// Check program initializations
	if (!programInitialized) {
		
		logger.debug("Program initialization failed !");
		MessageBoxA(nullptr, message.c_str(), nullptr, 0);
	} else {
		logger.debug("Program initialized.");
	}
	return programInitialized;
}

bool initTranslator(Translator* translator, string& message) 
{
	logger.debug("Init Translator...");
	try {
		translator->addLanguage(Translator::LANG_EN, "en.lang");
	} catch (const exception& e) {
		message += e.what();
		message += "\n";
	}

	try {
		translator->addLanguage(Translator::LANG_FR, "fr.lang");
	} catch (const exception& e) {
		message += e.what();
		message += "\n";
	}

	const auto isInitialized = translator->isInitialized();
	if (!isInitialized) {
		string errorMessage = "Translator is not initialized : No language file available.";
		logger.error(errorMessage);
		message += errorMessage + "\n";
	}

	return isInitialized;
}