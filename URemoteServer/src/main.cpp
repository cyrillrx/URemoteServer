#include "AI.h"
#include "ServerConfig.h"
#include "Translator.h"
#include <Windows.h>
#include <iostream>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

bool initTranslator(Translator* translator) 
{
	try {
		translator->addLanguage(Translator::LANG_EN, "en.lang");
	} catch (const exception& e) {
		MessageBoxA(nullptr, e.what(), nullptr, 0);
	}

	try {
		translator->addLanguage(Translator::LANG_FR, "fr.lang");
	} catch (const exception& e) {
		MessageBoxA(nullptr, e.what(), nullptr, 0);
	}

	return (translator->countLanguages() > 0);
}

bool initProgram()
{
	// TODO: Init translator
	// TODO: Init logger
	// TODO: Init ai
	// TODO: Init server
	return false;
}

int main()
{
	auto translator = Translator::getInstance();
	if (!initTranslator(translator)) {
		MessageBoxA(nullptr, "No language file available.\nProgram will shutdown.", nullptr, 0);
		return EXIT_FAILURE;
	}
	
	// Init config for the AI
	auto aiConfig = unique_ptr<AIConfig>(new AIConfig("ai.conf"));
	translator->setLanguage(aiConfig->Lang);

	// Create the AI
	auto artificialIntelligence = unique_ptr<AI>(new AI(move(aiConfig)));
	
	// Init config for the server
	auto serverConfig = unique_ptr<ServerConfig>(new ServerConfig("server.conf"));

	// Start the server on the AI
	artificialIntelligence->StartConnection(move(serverConfig));
	
	Translator::freeInstance();
    return EXIT_SUCCESS;
}