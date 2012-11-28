#include "AI.h"
#include "ServerConfig.h"
#include <memory>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	// Init config for the AI
	auto aiConfig = unique_ptr<AIConfig>(new AIConfig("ai.conf"));
	// Create the AI
	auto artificialIntelligence = unique_ptr<AI>(new AI(move(aiConfig)));

	// Init config for the server
	auto serverConfig = unique_ptr<ServerConfig>(new ServerConfig("server.conf"));
	// Start the server on the AI
	artificialIntelligence->StartConnection(move(serverConfig));

    return EXIT_SUCCESS;
}