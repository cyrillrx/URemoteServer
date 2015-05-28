#pragma once

#include <string>
#include <ctime>
#include <memory>
#include <vector>

#include "ai_config.h"
#include "authorized_users.h"
#include "listeners/Listener.h"

// TODO update Biicode config
//#include "network_io.h"
#include "cyrillrx/cross_api/src/network_io.h"
#include "network_io/server_config.h"

class Server;

class AI : public std::enable_shared_from_this<AI>
{
public:
    AI(std::unique_ptr<ai_config> aiConfig, std::unique_ptr<authorized_users> users);

    ~AI();

    void startConnection(std::unique_ptr<network_io::server_config> serverConfig);

    void stopConnection();

    bool isAuthorized(const std::string& securityToken);

    std::string getUser(const std::string& securityToken);

    std::string getName();

    void welcome(const std::string& securityToken);

    void say(const std::string& textToSpeak, const bool& text_only = false);

    bool toggleMute();

private:
    std::unique_ptr<ai_config> config_;
    std::unique_ptr<authorized_users> users_;
    std::unique_ptr<text_to_speech::voice> voice_;

    std::vector<std::unique_ptr<Listener>> listeners_;
    time_t lastWelcome_;


    AI(const AI&);

    const AI& operator=(const AI&);

    void start();

    void shutdown();
};
