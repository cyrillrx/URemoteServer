#include "MessageManager.h"
/*

//#include <stdlib.h>
#include <sstream>
#include "Message.h"
#include "MasterVolume.h"
#include "Keyboard.h"
#include "App.h"
#include "FileManager.h"
#include "MonUtils.h"
#include "Speech.h"

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

MessageManager* MessageManager::s_MessageManager = NULL; 

MessageManager::MessageManager(void)
{
}


MessageManager::~MessageManager(void)
{
}

//! Traitement de la commande envoyée par le client
string MessageManager::TreatCommand(string _cmd) 
{
	cout << "Server::TreatCommand : Command \"" << _cmd << "\" received." << endl;
	size_t position = _cmd.find ('|');

	string code, param;
	if (position != string::npos) { // Si on trouve un séparateur
		code = _cmd.substr(0, position);
		param = _cmd.substr(position + 1);
	} else {
		code = _cmd;
		param = "";
	}
	cout << "Code is " << code << endl;

	ostringstream reply;
	
	if (code == Message::CODE_CLASSIC) {
		Reply(ClassicCommand(param));
		return;
		
	// Commande des applications
	} else if (code == Message::CODE_APP) {
		Reply(AppCommand(param));
		return;
	
	// Commande clavier classique
	} else if (code == Message::CODE_KEYBOARD) {
		Reply(Keyboard::Command(param));
		return;
	
	// Combinaison de commandes clavier
	} else if (code == Message::CODE_KEYBOARD_COMBO) {
		Reply(Keyboard::Combo(param));
		return;
	
	// Media 
	} else if (code == Message::CODE_MEDIA) {
		Reply(Keyboard::MediaCommand(param));
		return;
	
	// Commande de volume
	} else if (code == Message::CODE_VOLUME) {
		Reply(VolumeCommand(param));
		return;

	// Ouvrir un dossier
	} else if (code == Message::OPEN_DIR) {
		reply << FileManager::ListFilesStr(param);

	// Ouvrir un fichier
	} else if (code == Message::OPEN_FILE) { 
		ShellExecuteA(NULL, NULL, param.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
		reply << "Ouverture du fichier : " << param;

	} else if (_cmd == "\0") {
		reply << "No command had been receive";

	} else {
		reply << "Unknown code received !";
		return;
	}

	Reply(reply.str());
}

//! Traitement d'un commande de volume
string MessageManager::VolumeCommand(string _param)
{
	ostringstream reply;

	// Volume +
	if (_param == Message::VOLUME_UP) {
		float fVolumeLvl = MasterVolume::GetInstance()->TurnUp();
		MasterVolume::FreeInstance();

		reply << "Volume up to " << fVolumeLvl * 100 << "%";
			
	// Volume -
	} else if (_param == Message::VOLUME_DOWN) {
		float fVolumeLvl = MasterVolume::GetInstance()->TurnDown();
		MasterVolume::FreeInstance();

		//sprintf_s(buffer, "Volume is %f", fVolumeLvl);
		reply << "Volume down to " << fVolumeLvl * 100 << "%";
			
	// Volume Mute
	} else if (_param == Message::VOLUME_MUTE) {
		bool isMute = MasterVolume::GetInstance()->ToggleMute();
		MasterVolume::FreeInstance();
			
		if (isMute) {
			reply << "Volume is off.";
		} else {
			reply << "Volume is now on.";
		}

	} else {
		return "Unknown Volume command !";
	}
	const string replyStr = reply.str();
	Speech::Say(replyStr);
	return replyStr;
}

//! Traitement d'une commande général
string MessageManager::ClassicCommand(string _param)
{
	ostringstream reply;

	if (_param == Message::HELLO_SERVER) {
		reply << Message::HELLO_CLIENT;
		Speech::SayW(L"Cyril is now connected. Welcome to Kronos System sir.");

	// Commande de test
	} else if (_param == Message::TEST_COMMAND) {
		reply << "The code has been tested";
		//Speech::Say(L"Code has been tested");
		//Speech::Say(reply.str());
		Speech::Say(reply.str().c_str());

	// Commande pour tuer le serveur
	} else if (_param == Message::KILL_SERVER) {
		m_ContinueToListen = false;
		reply << "Server killed";

	// Switch écran
	} else if (_param == Message::MONITOR_SWITCH_WINDOW) {
		MonUtils::SwitchWindow();
		reply << "Switch window";
			
	} else {
		return "Unknown classic command !";
	}

	return reply.str();
}

//! Traitement d'une commande applicative
string Server::AppCommand(string _param)
{
	ostringstream reply;

	// Ouvrir Gom player
	if (_param == Message::APP_GOM_PLAYER) {
		cout << Message::APP_GOM_PLAYER << endl;
		App* gomPlayer = new App("Gom Player", "GomPlayer1.x", "D:\\Programs\\GomPlayer\\GOM.exe");
		string resultMessage = gomPlayer->Show();
		delete gomPlayer;
		gomPlayer = NULL;
		reply << resultMessage;
		
	// Fermer Gom player
	} else if (_param == Message::KILL_GOM_PLAYER) {
		App* gomPlayer = new App("Gom Player", "GomPlayer1.x", "D:\\Programs\\GomPlayer\\GOM.exe");
		string resultMessage = gomPlayer->Close();
		delete gomPlayer;
		gomPlayer = NULL;
		reply << resultMessage;

	// Stretch Gom player
	} else if (_param == Message::GOM_PLAYER_STRETCH) {
		App* gomPlayer = new App("Gom Player", "GomPlayer1.x", "D:\\Programs\\GomPlayer\\GOM.exe");
		string resultMessage = gomPlayer->Strech();
		delete gomPlayer;
		gomPlayer = NULL;
		reply << resultMessage;
			
	} else {
		return "Unknown app command !";
	}
	
	Speech::Say(reply.str().c_str());
	return reply.str();
}
//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

MessageManager* MessageManager::GetInstance() {
	if (!s_MessageManager) {
		s_MessageManager = new MessageManager();
	}
	return s_MessageManager;
}

void MessageManager::FreeInstance() {
	delete(s_MessageManager);
	s_MessageManager = NULL;
}
*/