#include "AI.h"
#include "ServerConfig.h"
#include "Translator.h"
#include "Utils.h"
#include <Windows.h>
#include <iostream>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

bool initProgram(unique_ptr<AIConfig>& aiConfig, unique_ptr<ServerConfig>& serverConfig);
bool initTranslator(Translator* translator, string& message);

Logger* logger = Utils::getLogger();

int main()
{
	// TODO: Init Logger
	logger->setLogFile("URemote.log");
	// TODO: init structure (files and directories)

	logger->debug("******************************************************");
	logger->debug("*****               URemote Server               *****");
	logger->debug("******************************************************");

	unique_ptr<AIConfig> aiConfig = nullptr;
	unique_ptr<ServerConfig> serverConfig = nullptr;
	if (!initProgram(aiConfig, serverConfig)) {
		return EXIT_FAILURE;
	}
	
	// Create the AI
	auto artificialIntelligence = unique_ptr<AI>(new AI(move(aiConfig)));
	
	// Start the server on the AI
	artificialIntelligence->StartConnection(move(serverConfig));
	
	Translator::freeInstance();
    return EXIT_SUCCESS;
}

bool initProgram(unique_ptr<AIConfig>& aiConfig, unique_ptr<ServerConfig>& serverConfig)
{
	logger->debug("Program initialization...");

	bool programInitialized = false;
	string message = "";
	
	// Init config for the AI
	logger->debug("Init config for the AI...");
	bool aiInitialized = false;
	try {
		aiConfig = unique_ptr<AIConfig>(new AIConfig(".\\conf\\ai.conf"));
		logger->debug("AI config OK.");
		aiInitialized = true;
	} catch (const exception& e) {
		message += e.what();
		message += "\n";
		logger->debug("AI config KO.");
	}

	// Init translator
	auto translator = Translator::getInstance();
	bool translatorInitialized = initTranslator(translator, message);
	if (translatorInitialized && aiInitialized) {
		translator->setLanguage(aiConfig->Lang);
	}

	// Init config for the server
	logger->debug("Init config for the server...");
	bool serverInitialized = false;
	try {
		serverConfig = unique_ptr<ServerConfig>(new ServerConfig(".\\conf\\server.conf"));
		logger->debug("Server config OK.");
		serverInitialized = true;
	} catch (const exception& e) {
		logger->error(e.what());
		message += e.what();
		message += "\n";
		logger->debug("Server config KO.");
	}

	programInitialized = aiInitialized && translatorInitialized && serverInitialized;

	// Check program initializations
	if (programInitialized) {
		logger->debug("Program initialized.");
	} else {
		logger->error("Program initialization failed !");
		MessageBoxA(nullptr, message.c_str(), nullptr, 0);
	}

	return programInitialized;
}

bool initTranslator(Translator* translator, string& message) 
{
	logger->debug("Init Translator...");
	try {
		translator->addLanguage(Translator::LANG_EN, ".\\conf\\en.lang");
	} catch (const exception& e) {
		logger->warning(e.what());
		message += e.what();
		message += "\n";
	}

	try {
		translator->addLanguage(Translator::LANG_FR, ".\\conf\\fr.lang");
	} catch (const exception& e) {
		logger->warning(e.what());
		message += e.what();
		message += "\n";
	}

	const auto isInitialized = translator->isInitialized();
	if (isInitialized) {
		logger->debug("Translator OK.");
	} else {
		string errorMessage = "Translator KO : No language file available.";
		logger->error(errorMessage);
		message += errorMessage + "\n";
	}

	return isInitialized;
}