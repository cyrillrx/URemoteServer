#pragma once

#include <string>
#include <ctime>
#include <memory>
#include <vector>

#include "ai_config.h"
#include "network_io/server_config.h"
#include "listeners/Listener.h"

class Speech;
class Server;

class AI : public std::enable_shared_from_this<AI>
{
public:
	AI(std::unique_ptr<ai_config> aiConfig);
	~AI();

	void startConnection(std::unique_ptr<network_io::server_config> serverConfig);
	void stopConnection();

	std::string getName();

	void welcome();
	void say(std::string textToSpeak);
	bool toggleMute();

private:
	std::unique_ptr<ai_config> config_;
	std::unique_ptr<Speech> voice_;

	std::vector<std::unique_ptr<Listener>> listeners_;
	time_t lastWelcome_;


	AI(const AI&);
	const AI& operator=(const AI&);

	void start();
	void shutdown();
};
