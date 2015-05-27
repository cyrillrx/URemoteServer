#include <iostream>

#include "logger/console_logger.h"
#include "string_utils.h"
#include "lang/lexicon_manager.h"
// TODO update Biicode config
//#include "network_io/serialized_message.h"
#include "cyrillrx/cross_api/src/network_io/serialized_message.h"

#include "AI.h"
#include "executor.h"
#include "trad_key.h"
#include "modules/App.h"
#include "modules/file_manager.h"
#include "modules/keyboard.h"
#include "modules/master_volume.h"
#include "modules/MonUtils.h"

using namespace network_io;

auto log = ConsoleLogger(DEBUG);

executor::executor(std::shared_ptr<AI> ai)
        : ai_(ai) { }

serialized_message executor::handle_request(serialized_message serializedRequest) const
{
    const auto request = build_request(serializedRequest);

    const auto reqType = request.type();
    const auto reqCode = request.code();
    const auto securityToken = request.securitytoken();
    const auto strExtra = request.stringextra();
    const auto intExtra = request.intextra();

    log.Info("executor::handle_request() Received message : ");
    log.Info(" - Type      <" + Request_Type_Name(reqType) + ">");
    log.Info(" - Code      <" + Request_Code_Name(reqCode) + ">");
    log.Info(" - Token     <" + securityToken + ">");
    log.Info(" - str extra <" + strExtra + ">");
    log.Info(" - int extra <" + std::to_string(intExtra) + ">");

    Response reply;
    reply.set_requesttype(reqType);
    reply.set_requestcode(reqCode);

    if (!ai_->isAuthorized(securityToken)) {
        reply.set_returncode(Response_ReturnCode_RC_ERROR);
        const auto message = lexicon_manager::get_string(trad_key::XC_UNKNOWN_SECURITY_TOKEN);
        reply.set_message(message);

        ai_->say(message);

    } else {

        switch (reqType) {

            case Request_Type_SIMPLE:
                classic_command(reply, reqCode, securityToken);
                break;

            case Request_Type_EXPLORER:
                file_manager::handle_message(&reply, reqCode, strExtra);
                break;

            case Request_Type_KEYBOARD:
                keyboard::handle_request(request, reply);
                break;

            case Request_Type_AI:
                ai_command(reply, reqCode);
                break;

            case Request_Type_VOLUME:
                volume_command(reply, reqCode, intExtra);
                break;

            case Request_Type_APP:
                app_command(reply, reqCode);
                break;

            default:
                reply.set_returncode(Response_ReturnCode_RC_ERROR);
                reply.set_message("Unknown type received : " + Request_Type_Name(reqType));
                break;
        }
    }

    int bufSize = 0;
    char *buf = nullptr;

    try {
        return serialize_response(reply);
    } catch (const std::exception &e) {
        log.Error("executor::handle_request() : " + std::string(e.what()));
        return serialize_response(clear_response(reply, std::string(e.what())));
    }

}


/** Handle general commands. */
void executor::classic_command(Response &reply, const Request_Code &code, const std::string &securityToken) const
{
    switch (code) {
        case Request_Code_PING:
            reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
            ai_->welcome(securityToken);
            break;

        case Request_Code_TEST:
            reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
            reply.set_message(lexicon_manager::get_string(trad_key::XC_TEST));
            break;

        case Request_Code_KILL_SERVER:
            reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
            reply.set_message("Server killed");
            ai_->stopConnection();
            break;

        case Request_Code_SHUTDOWN:
            shutdown_pc(reply, 10);
            break;

        case Request_Code_LOCK:
# if defined(WINDOWS_PLATFORM)
		LockWorkStation();
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("PC locked");
# else
            reply.set_returncode(Response_ReturnCode_RC_ERROR);
            reply.set_message("Not implemented on Linux");
# endif
            break;

        case Request_Code_SWITCH_WINDOW:
            if (MonUtils::SwitchWindow()) {
                reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
                reply.set_message("Switch window");
            } else {
                reply.set_returncode(Response_ReturnCode_RC_ERROR);
                reply.set_message("Switch window error");
            }
            break;

        default:
            reply.set_returncode(Response_ReturnCode_RC_ERROR);
            reply.set_message("Unknown Simple Command received : " + Request_Code_Name(code));
            break;
    }
}

/** Handle volume commands. */
void executor::volume_command(Response &reply, const Request_Code &code, const int &intExtra) const
{
    float fVolumeLvl;
    bool isMute;

    const int bufferlength = 50;
    char buffer[bufferlength];
    std::string message;
    int volumePercent;

    switch (code) {

        case Request_Code_DEFINE:
            fVolumeLvl = master_volume::getInstance()->define(intExtra);
            master_volume::freeInstance();

            reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
            volumePercent = (int) (fVolumeLvl * 100);
            reply.set_intvalue(volumePercent);

            string_utils::securedPrint(buffer, bufferlength, "Volume set to %d%%", volumePercent);
            message = buffer;
            break;

        case Request_Code_DPAD_UP:
            fVolumeLvl = master_volume::getInstance()->turnUp();
            master_volume::freeInstance();

            reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
            volumePercent = (int) (fVolumeLvl * 100);
            reply.set_intvalue(volumePercent);

            string_utils::securedPrint(buffer, bufferlength, "Volume set to %d%%", volumePercent);
            message = buffer;
            break;

        case Request_Code_DPAD_DOWN:
            fVolumeLvl = master_volume::getInstance()->turnDown();
            master_volume::freeInstance();

            reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
            volumePercent = (int) (fVolumeLvl * 100);
            reply.set_intvalue(volumePercent);

            string_utils::securedPrint(buffer, bufferlength, "Volume set to %d%%", volumePercent);
            message = buffer;
            break;

        case Request_Code_MUTE:
            isMute = master_volume::getInstance()->toggleMute();
            master_volume::freeInstance();

            reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
            reply.set_intvalue(isMute);
            message = (isMute) ? "Volume is off." : "Volume is now on.";
            break;

        default:
            message = "Unknown Volume command !";
            log.Error(message);
            break;
    }

    reply.set_message(message);
}

/** Handle AI commands */
void executor::ai_command(Response &reply, const Request_Code &code) const
{
    bool isMute;
    std::string message;

    switch (code) {

        case Request_Code_MUTE:
            isMute = ai_->toggleMute();

            reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
            message = (isMute) ? "AI volume is off." : "AI volume is now on.";
            break;

        default:
            reply.set_returncode(Response_ReturnCode_RC_ERROR);
            message = "Unknown AI command !";
            log.Error(message);
            break;
    }

    reply.set_message(message);
}

/** Handle application commands. */
void executor::app_command(Response &reply, const Request_Code &code) const
{
    std::string message;

    switch (code) {

        // Start the specified app
        case Request_Code_ON:
            reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
            message = App::GetGomPlayer()->Show();
            App::FreeGomPlayer();
            break;

            // Close the specified app
        case Request_Code_OFF:
            reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
            message = App::GetGomPlayer()->Close();
            App::FreeGomPlayer();
            break;

            // Stretch Gom player
        case Request_Code_KEYCODE_0:
            reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
            message = App::GetGomPlayer()->Stretch();
            App::FreeGomPlayer();
            break;

        default:
            reply.set_returncode(Response_ReturnCode_RC_ERROR);
            message = "Unknown app command !";
            log.Error(message);
            break;
    }

    reply.set_message(message);
}

/** Send a command to shutdown the computer. */
void executor::shutdown_pc(Response &reply, const int &delay) const
{
    ai_->stopConnection();

    const auto message = lexicon_manager::get_string(trad_key::XC_PC_SHUTDOWN, delay);

    const std::string command = "Shutdown.exe -s -t " + std::to_string(delay) + " -c \"" + message + "\"";
    system(command.c_str());

    reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
    reply.set_message(message);

    ai_->say(message);
}
