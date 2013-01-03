#include "AI.h"
#include "ServerConfig.h"
#include "Translator.h"
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	auto translator = Translator::getInstance();
	translator->AddLanguage(Translator::LANG_EN, "en.lang");
	translator->AddLanguage(Translator::LANG_FR, "fr.lang");
	
	// Init config for the AI
	auto aiConfig = unique_ptr<AIConfig>(new AIConfig("ai.conf"));
	translator->SetLanguage(aiConfig->Lang);

	// Create the AI
	auto artificialIntelligence = unique_ptr<AI>(new AI(move(aiConfig)));
	
	// Init config for the server
	auto serverConfig = unique_ptr<ServerConfig>(new ServerConfig("server.conf"));

	// Start the server on the AI
	artificialIntelligence->StartConnection(move(serverConfig));
	
	Translator::freeInstance();
    return EXIT_SUCCESS;
}