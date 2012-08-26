#include "Exchange.h"

#include <iostream>
#include <sstream>
#include <ostream>

#include "AI.h"
#include "FileManager.h"
#include "MonUtils.h"
#include "MasterVolume.h"
#include "Keyboard.h"
#include "App.h"

// Gestion des applications
const string Exchange::APP_GOM_PLAYER	= "app_gom_player";
const string Exchange::KILL_GOM_PLAYER	= "kill_gom_player";
const string Exchange::GOM_PLAYER_STRETCH	= "gom_player_stretch";

string Exchange::HandleMessage(void* _data, bool &_continueToListen)
{
	Request* request = new Request();
	request->ParseFromArray(_data, sizeof(_data));

	Request_Type reqType	= request->type();
	Request_Code reqCode	= request->code();
	string param	= request->stringparam();
	
	cout << "Server::HandleMessage() Received message : " << endl;
	cout << " - Type	<" << Request_Type_Name(reqType)	<< ">" << endl;
	cout << " - Code	<" << Request_Code_Name(reqCode)	<< ">" << endl;
	cout << " - Param	<" << param							<< ">" << endl;

	Response *reply = new Response();
	switch (reqType) {
	
	case Request_Type_SIMPLE:
		ClassicCommand(reply, reqCode);
		break;

	case Request_Type_EXPLORER:
		FileManager::HandleMessage(reply, reqCode, param);
		break;
		
	case Request_Type_KEYBOARD:
		Keyboard::HandleMessage(reply, reqCode, param);
		break;

	case Request_Type_AI:
		AICommand(reply, reqCode);
		break;
		
	case Request_Type_VOLUME:
		VolumeCommand(reply, reqCode);
		break;

	// Commande des applications
	case Request_Type_APP:
		AppCommand(reply, reqCode);
		break;

	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("Unknown type received : " + reqType);
		break;
	}
	
	if (reply->has_message()) {
		AI::GetInstance()->Say(reply->message());
	}

	string serializedReply;
	reply->SerializeToString(&serializedReply);
	
	delete(reply);
	reply = NULL;

	return serializedReply;
}

//! Traitement d'une commande général
void Exchange::ClassicCommand(Response* _reply, Request_Code _code)
{
	switch (_code) {

	case Request_Code_HELLO:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Hello back.");
		AI::GetInstance()->Welcome();
		break;
		
	case Request_Code_TEST:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		//_reply->set_message("The code has been tested");
		_reply->set_message("No way. Go Fuck yourself !");
		break;

	case Request_Code_KILL_SERVER:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Server killed");
		AI::GetInstance()->StopConnection();
		break;

	case Request_Code_SHUTDOWN:
		ShutdownPC(_reply, 10);
		break;

	case Request_Code_SWITCH_WINDOW:
		if (MonUtils::SwitchWindow()) {
			_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
			_reply->set_message("Switch window");
		} else {
			_reply->set_returncode(Response_ReturnCode_RC_ERROR);
			_reply->set_message("Switch window error");
		}
		break;
			
	default:
		_reply->set_returncode(Response_ReturnCode_RC_ERROR);
		_reply->set_message("Unknown ClassicCommand received : " + _code);
		break;
	}
}

//! Traitement d'un commande de volume
void Exchange::VolumeCommand(Response* _reply, Request_Code _code)
{
	float fVolumeLvl;
	bool isMute;
	char* message = "";
	int volumePoucentage;

	switch (_code) {

	case Request_Code_UP:
		fVolumeLvl = MasterVolume::GetInstance()->TurnUp();
		MasterVolume::FreeInstance();

		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = fVolumeLvl * 100;
		_reply->set_intvalue(volumePoucentage);
		sprintf(message,  "Volume up to %d%", volumePoucentage);
		break;
			
	case Request_Code_DOWN:
		fVolumeLvl = MasterVolume::GetInstance()->TurnDown();
		MasterVolume::FreeInstance();

		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = fVolumeLvl * 100;
		_reply->set_intvalue(volumePoucentage);
		sprintf(message,  "Volume down to %d%", volumePoucentage);
		break;
			
	case Request_Code_MUTE:
		isMute = MasterVolume::GetInstance()->ToggleMute();
		MasterVolume::FreeInstance();
		
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);	
		_reply->set_intvalue(isMute);
		message = (isMute) ? "Volume is off." : "Volume is now on.";
		break;
	
	default:
		message = "Unknown Volume command !";
		cerr << message << endl;
		break;
	}

	_reply->set_message(message);
}

/** Handle AI commands */
void Exchange::AICommand(Response* _reply, Request_Code _code)
{
	bool isMute;
	char* message;

	switch (_code) {

	case Request_Code_MUTE:
		isMute = AI::GetInstance()->ToggleMute();
			
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = (isMute) ? "AI volume is off." : "AI volume is now on.";
		break;
	
	default:
		_reply->set_returncode(Response_ReturnCode_RC_ERROR);
		message = "Unknown AI command !";
		cerr << message << endl;
		break;
	}
	
	_reply->set_message(message);
}

/** Handle application commands. */
void Exchange::AppCommand(Response* _reply, Request_Code _code)
{
	string message;

	switch (_code) {

	// Ouvrir Gom player
	case Request_Code_GOM_PLAYER_RUN:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Show();
		App::FreeGomPlayer();
		break;
		
	// Fermer Gom player
	case Request_Code_GOM_PLAYER_KILL:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Close();
		App::FreeGomPlayer();
		break;

	// Stretch Gom player
	case Request_Code_GOM_PLAYER_STRETCH:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Strech();
		App::FreeGomPlayer();
		break;
			
	default:
		_reply->set_returncode(Response_ReturnCode_RC_ERROR);
		message = "Unknown app command !";
		cerr << message << endl;
		break;
	}
	
	_reply->set_message(message);
}

/** Send a command to shutdown the computer. */
void Exchange::ShutdownPC(Response* _reply, int _delay)
{
	AI::GetInstance()->StopConnection();
	//ShellExecute(NULL, L"shutdown", NULL, L"-s -t 10", NULL, SW_SHOWMAXIMIZED);
	
	char* command = "";
	sprintf(command, "Shutdown.exe -s -t %d -c \"L'ordinateur va s'éteindre dans %d secondes\"", _delay, _delay);
	system(command);

	_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
	char* message = "";
	sprintf(message, "PC will shutdown in %d seconds", _delay);
	_reply->set_message(message);

	AI::GetInstance()->Say(message);
}
/*
string Exchange::GetRequestType(Request_Type _type)
{
	switch (_type) {
	case Request_Type_SIMPLE:
		return "Request_Type_SIMPLE";
	case Request_Type_EXPLORER:
		return "Request_Type_EXPLORER";
	case Request_Type_KEYBOARD:
		return "Request_Type_KEYBOARD";
	case Request_Type_AI:
		return "Request_Type_AI";
	case Request_Type_VOLUME:
		return "Request_Type_VOLUME";
	case Request_Type_APP:
		return "Request_Type_APP";
	default:
		return "Error missing Request_Type value.";
	}
}

string Exchange::GetRequestCode(Request_Code _code)
{
	switch (_code) {
	case Request_Code_DEFINE:
		return "Request_Code_DEFINE";
	case Request_Code_HELLO:
		return "Request_Code_HELLO";
	case Request_Code_TEST:
		return "Request_Code_TEST";
	case Request_Code_KILL_SERVER:
		return "Request_Code_KILL_SERVER";
	case Request_Code_SHUTDOWN:
		return "Request_Code_SHUTDOWN";
	case Request_Code_SWITCH_WINDOW:
		return "Request_Code_SWITCH_WINDOW";
	case Request_Code_LOCK:
		return "Request_Code_LOCK";
	case Request_Code_UP:
		return "Request_Code_UP";
	case Request_Code_DOWN:
		return "Request_Code_DOWN";
	case Request_Code_LEFT:
		return "Request_Code_LEFT";
	case Request_Code_RIGHT:
		return "Request_Code_RIGHT";
	case Request_Code_MUTE:
		return "Request_Code_MUTE";
	case Request_Code_SAY:
		return "Request_Code_SAY";
	case Request_Code_GET_FILE_LIST:
		return "Request_Code_GET_FILE_LIST";
	case Request_Code_OPEN_FILE:
		return "Request_Code_OPEN_FILE";
	case Request_Code_MEDIA_PLAY_PAUSE:
		return "Request_Code_MEDIA_PLAY_PAUSE";
	case Request_Code_MEDIA_STOP:
		return "Request_Code_MEDIA_STOP";
	case Request_Code_MEDIA_PREVIOUS:
		return "Request_Code_MEDIA_PREVIOUS";
	case Request_Code_MEDIA_NEXT:
		return "Request_Code_MEDIA_NEXT";
	case Request_Code_MEDIA_FF:
		return "Request_Code_MEDIA_FF";
	case Request_Code_MEDIA_REWIND:
		return "Request_Code_MEDIA_REWIND";
	case Request_Code_KB_RETURN:
		return "Request_Code_KB_RETURN";
	case Request_Code_KB_SPACE:
		return "Request_Code_KB_SPACE";
	case Request_Code_KB_BACKSPACE:
		return "Request_Code_KB_BACKSPACE";
	case Request_Code_KB_ESCAPE:
		return "Request_Code_KB_ESCAPE";
	case Request_Code_KB_ALT_F4:
		return "Request_Code_KB_ALT_F4";
	case Request_Code_KB_CTRL_RETURN:
		return "Request_Code_KB_CTRL_RETURN";
	default:
		return "Error missing Request_Code value.";
	}
}
*/