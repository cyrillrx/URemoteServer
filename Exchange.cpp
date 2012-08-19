#include "Exchange.h"

#include <iostream>
#include <sstream>

#include "FileManager.h"
#include "MonUtils.h"
#include "AI.h"
#include "MasterVolume.h"
#include "Keyboard.h"

// Gestion des applications
const string Exchange::APP_GOM_PLAYER	= "app_gom_player";
const string Exchange::KILL_GOM_PLAYER	= "kill_gom_player";
const string Exchange::GOM_PLAYER_STRETCH	= "gom_player_stretch";

string Exchange::HandleMessage(string _msg, bool &_continueToListen)
{
	cout << "Server::HandleMessage() : Command <" << _msg << ">" << endl;

	Request request;
	request.ParseFromString(_msg);

	Request_Type reqType	= request.type();
	Request_Code reqCode	= request.code();
	string param	= request.text();

	cout << "	Type	<" << Request_Type_Name(reqType)	<< ">" << endl;
	cout << "	Code	<" << Request_Code_Name(reqCode)	<< ">" << endl;
	cout << "	Param	<" << param							<< ">" << endl;

	Response *reply;
	switch (reqType) {
	
	case Request_Type_SIMPLE:
		reply = ClassicCommand(reqCode);
		break;

	case Request_Type_EXPLORER:
		reply = FileManager::HandleMessage(reqCode, param);
		break;
		
	case Request_Type_KEYBOARD:
		reply = FileManager::HandleMessage(reqCode, param);
		break;

	case Request_Type_AI:
		reply = AICommand(reqCode);
		break;
		
	case Request_Type_VOLUME:
		reply = VolumeCommand(reqCode);
		break;

	// Commande des applications
	case Request_Type_APP:
		AppCommand(param);
		break;

	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("Unknown type received : " + reqType);
	}
}

//! Traitement d'une commande général
Response* Exchange::ClassicCommand(Request_Code _code)
{
	Response *reply = new Response();
	reply->set_returncode(Response_ReturnCode_RC_SUCCESS);

	switch (_code) {

	case Request_Code_HELLO:
		reply->set_message("Hello back.");
		AI::GetInstance()->Welcome();
		break;
		
	case Request_Code_TEST:
		string text = "The code has been tested";
		string text = "No way. Go Fuck yourself !";
		reply->set_message(text);
		AI::GetInstance()->Say(text);
		break;

	case Request_Code_KILL_SERVER:
		reply->set_message("Server killed");
		AI::GetInstance()->StopConnection();
		break;

	case Request_Code_SHUTDOWN:
		reply = ShutdownPC(10);
		break;

	case Request_Code_SWITCH_WINDOW:
		MonUtils::SwitchWindow();
		reply->set_message("Switch window");
		break;
			
	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("ClassicCommand :Unknown code received : " + _code);
		break;
	}

	return reply;
}

//! Traitement d'un commande de volume
Response* Exchange::VolumeCommand(Request_Code _code)
{
	Response *reply = new Response();
	reply->set_returncode(Response_ReturnCode_RC_SUCCESS);

	float fVolumeLvl;
	bool isMute;

	switch (_code) {

	case Request_Code_UP:
		fVolumeLvl = MasterVolume::GetInstance()->TurnUp();
		MasterVolume::FreeInstance();
		reply << "Volume up to " << fVolumeLvl * 100 << "%";
		break;
			
	case Request_Code_DOWN:
		fVolumeLvl = MasterVolume::GetInstance()->TurnDown();
		MasterVolume::FreeInstance();
		reply << "Volume down to " << fVolumeLvl * 100 << "%";
		break;
			
	case Request_Code_MUTE:
		isMute = MasterVolume::GetInstance()->ToggleMute();
		MasterVolume::FreeInstance();
			
		if (isMute) {
			reply << "Volume is off.";
		} else {
			reply << "Volume is now on.";
		}
		break;
	
	default:
		reply << "Unknown Volume command !";
		break;
	}
	const string replyStr = reply.str();
	m_ArtificialIntelligence->Say(replyStr);
	return replyStr;
}

/** Send a command to shutdown the computer. */
Response* Exchange::ShutdownPC(int _delay)
{
	Response *reply = new Response();
	reply->set_returncode(Response_ReturnCode_RC_SUCCESS);

	AI::GetInstance()->StopConnection();
	//ShellExecute(NULL, L"shutdown", NULL, L"-s -t 10", NULL, SW_SHOWMAXIMIZED);
	string command = "Shutdown.exe -s -t " +_delay + " -c \"L'ordinateur va s'éteindre dans " +_delay + " secondes\"");
	system("Shutdown.exe -s -t " + _delay + " -c \"L'ordinateur va s'éteindre dans 10 secondes\"");
	
	string message = "PC will shutdown in " +_delay + " seconds";
	reply->set_message(message);
	AI::GetInstance()->Say(message);
	return reply;
}


/** Handle AI commands */
Response* Server::AICommand(Request_Code _code)
{
	ostringstream reply;
	bool isMute;

	switch (_code) {

	case Request_Code_MUTE:
		isMute = AI::GetInstance()->ToggleMute();
			
		if (isMute) {
			reply << "AI volume is off.";
		} else {
			reply << "AI volume is now on.";
			m_ArtificialIntelligence->Say(reply.str());
		}
		break;
	
	default:
		reply << "Unknown AI command !";
	}

	return reply.str();
}

/** Handle application commands. */
Response* Server::AppCommand(string _param)
{
	ostringstream reply;

	// Ouvrir Gom player
	if (_param == Exchange::APP_GOM_PLAYER) {
		cout << Exchange::APP_GOM_PLAYER << endl;
		reply << App::GetGomPlayer()->Show();
		App::FreeGomPlayer();
		
	// Fermer Gom player
	} else if (_param == Exchange::KILL_GOM_PLAYER) {
		reply << App::GetGomPlayer()->Close();
		App::FreeGomPlayer();

	// Stretch Gom player
	} else if (_param == Exchange::GOM_PLAYER_STRETCH) {
		reply << App::GetGomPlayer()->Strech();
		App::FreeGomPlayer();
			
	} else {
		return "Unknown app command !";
	}
	
	m_ArtificialIntelligence->Say(reply.str().c_str());
	return reply.str();
}

string Exchange::GetRequestType(Request_Type _type)
{
	switch (_type) {
	case Request_Type_SIMPLE:
		return "Request_Type_SIMPLE";
	case Request_Type_EXPLORER:
		return "Request_Type_EXPLORER";
	case Request_Type_KEYBOARD:
		return "Request_Type_KEYBOARD";
	case Request_Type_MEDIA:
		return "Request_Type_MEDIA";
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