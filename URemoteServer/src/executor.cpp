#include "executor.h"

#include <iostream>
#include <string>

#include "modules/file_manager.h"
#include "modules/MonUtils.h"
#include "modules/MasterVolume.h"
#include "modules/Keyboard.h"
#include "modules/App.h"
#include "lexicon_manager.h"
#include "trad_key.h"
#include "Utils.h"
#include "string_utils.h"

using namespace network_io;

executor::executor(std::shared_ptr<AI> ai)
	: ai_(ai) { }

serialized_message executor::handle_request(serialized_message serializedRequest)
{
	auto request = build_request(serializedRequest);

	const auto reqType	= request.type();
	const auto reqCode	= request.code();
	const auto reqExtraCode		= request.extracode();
	const auto securityToken	= request.securitytoken();
	const auto strParam	= request.stringparam();
	const auto intParam	= request.intparam();

	Utils::get_logger()->info("Server::HandleMessage() Received message : ");
	Utils::get_logger()->info(" - Type		<" + Request_Type_Name(reqType)	+ ">");
	Utils::get_logger()->info(" - Code		<" + Request_Code_Name(reqCode)	+ ">");
	Utils::get_logger()->info(" - ExtraCode	<" + Request_Code_Name(reqExtraCode) + ">");
	Utils::get_logger()->info(" - Token	    <" + securityToken				+ ">");
	Utils::get_logger()->info(" - str param	<" + strParam					+ ">");
	Utils::get_logger()->info(" - int param	<" + std::to_string(intParam)	+ ">");

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
			file_manager::handle_message(&reply, reqCode, strParam);
			break;

		case Request_Type_KEYBOARD:
			Keyboard::HandleMessage(&reply, &request);
			break;

		case Request_Type_AI:
			ai_command(reply, reqCode);
			break;

		case Request_Type_VOLUME:
			volume_command(reply, reqCode, intParam);
			break;

		case Request_Type_APP:
			app_command(reply, reqCode);
			break;

		default:
			reply.set_returncode(Response_ReturnCode_RC_ERROR);
			reply.set_message("Unknown type received : " + reqType);
			break;
		}
	}

	int bufSize = 0;
	char* buf = nullptr;
	auto serializedReply = serialize_response(reply);

	return serializedReply;
}



/** Handle general commands. */
void executor::classic_command(Response& reply, const Request_Code& code, const std::string& securityToken)
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
		reply.set_message("Unknown Simple Command received : " + code);
		break;
	}
}

/** Handle volume commands. */
void executor::volume_command(Response& reply, const Request_Code& code, const int& intParam)
{
	float fVolumeLvl;
	bool isMute;

	const int bufferlength = 50;
	char buffer[bufferlength];
	std::string message;
	int volumePoucentage;

	switch (code) {

	case Request_Code_DEFINE:
		fVolumeLvl = MasterVolume::getInstance()->define(intParam);
		MasterVolume::freeInstance();

		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = (int) (fVolumeLvl * 100);
		reply.set_intvalue(volumePoucentage);

		string_utils::securedPrint(buffer, bufferlength, "Volume set to %d%%", volumePoucentage);
		message = buffer;
		break;

	case Request_Code_UP:
		fVolumeLvl = MasterVolume::getInstance()->turnUp();
		MasterVolume::freeInstance();

		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = (int) (fVolumeLvl * 100);
		reply.set_intvalue(volumePoucentage);

		string_utils::securedPrint(buffer, bufferlength, "Volume set to %d%%", volumePoucentage);
		message = buffer;
		break;

	case Request_Code_DOWN:
		fVolumeLvl = MasterVolume::getInstance()->turnDown();
		MasterVolume::freeInstance();

		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = (int) (fVolumeLvl * 100);
		reply.set_intvalue(volumePoucentage);

		string_utils::securedPrint(buffer, bufferlength, "Volume set to %d%%", volumePoucentage);
		message = buffer;
		break;

	case Request_Code_MUTE:
		isMute = MasterVolume::getInstance()->toggleMute();
		MasterVolume::freeInstance();

		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_intvalue(isMute);
		message = (isMute) ? "Volume is off." : "Volume is now on.";
		break;

	default:
		message = "Unknown Volume command !";
		Utils::get_logger()->error(message);
		break;
	}

	reply.set_message(message);
}

/** Handle AI commands */
void executor::ai_command(Response& reply, const Request_Code& code)
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
		Utils::get_logger()->error(message);
		break;
	}

	reply.set_message(message);
}

/** Handle application commands. */
void executor::app_command(Response& reply, const Request_Code& code)
{
	std::string message;

	switch (code) {

		// Ouvrir Gom player
	case Request_Code_GOM_PLAYER_RUN:
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Show();
		App::FreeGomPlayer();
		break;

		// Fermer Gom player
	case Request_Code_GOM_PLAYER_KILL:
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Close();
		App::FreeGomPlayer();
		break;

		// Stretch Gom player
	case Request_Code_GOM_PLAYER_STRETCH:
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Strech();
		App::FreeGomPlayer();
		break;

	default:
		reply.set_returncode(Response_ReturnCode_RC_ERROR);
		message = "Unknown app command !";
		Utils::get_logger()->error(message);
		break;
	}

	reply.set_message(message);
}

/** Send a command to shutdown the computer. */
void executor::shutdown_pc(Response& reply, const int& delay)
{
	ai_->stopConnection();

	const auto message = lexicon_manager::get_string(trad_key::XC_PC_SHUTDOWN, delay);

	const std::string command = "Shutdown.exe -s -t " + std::to_string(delay) + " -c \"" + message + "\"";
	system(command.c_str());

	reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
	reply.set_message(message);

	ai_->say(message);
}
