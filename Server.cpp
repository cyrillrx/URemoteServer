#include "Server.h"

#include <sstream>
#include <iostream>

#include "Message.h"
#include "MasterVolume.h"
#include "Keyboard.h"
#include "App.h"
#include "FileManager.h"
#include "MonUtils.h"
#include "Speech.h"

int Server::s_InstanceCount = 0;


//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

Server::Server(int _port, int _maxConnections)
{
	m_Port = _port;
	m_MaxConcurrentConnections = _maxConnections;
	InitServer();
}

Server::~Server(void)
{
	FreeServer();
}

//! Lancement du serveur
void Server::Launch()
{
	m_ContinueToListen = true;

	// Socket d'acceptation
	SOCKADDR_IN csin;
	char buffer[BUFSIZ];

	/* connection socket */
	int sizeofcsin = sizeof(csin);

	while(m_ContinueToListen) {
		cout << "Waiting for client to connect..." << endl;

		m_CSocket = accept(m_ListenSocket, (SOCKADDR *)&csin, &sizeofcsin);
		if (m_CSocket == INVALID_SOCKET) {
			cout << "accept failed with error: " << endl;
			cout << "accept failed with error: " << WSAGetLastError() << endl;
			FreeServer();
			return;
		}
		
		//memset(buffer, '\0', sizeof(buffer)); // On vide le buffer
		memset(buffer, '\0', BUFSIZ); // On vide le buffer
		//recv(m_CSocket, buffer, sizeof(buffer), 0);
		recv(m_CSocket, buffer, BUFSIZ, 0);
		cout << buffer << endl;
		string command = buffer;
		TreatCommand(command);
		
		closesocket(m_CSocket);
		cout << "Socket closed" << endl << endl;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

//! Initialisation du serveur
void Server::InitServer() 
{
	// Initialisation de la librairie de winSock
	WSADATA WSAData;
	int result = WSAStartup(MAKEWORD(2,0), &WSAData); // Utilisation de la version 2 de winsock
	if (result != NO_ERROR) {
		cout << "WSAStartup failed with error: " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	} else {
		s_InstanceCount++;
	}

	// Création du socket d'écoute des connexions entrantes
	m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (m_ListenSocket == INVALID_SOCKET) {
		cout << "socket failed with error: " << WSAGetLastError() << endl;
		FreeServer();
		return;
	}

	// Contient les informations techniques du socket
	SOCKADDR_IN socketAddress; 
	socketAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Adresse du serveur
	socketAddress.sin_family	= AF_INET; // famille de socket utilisée => Internet
	socketAddress.sin_port	= htons(m_Port);

	// Attache le socket à l'adresse et au port définis dans SOCKADDR 
	result = bind(m_ListenSocket, (SOCKADDR*)&socketAddress, sizeof(socketAddress));
	if (result == SOCKET_ERROR) {
		cout << "bind failed with error: " << WSAGetLastError() << endl;
		FreeServer();
		return;
	}

	// Ecoute des connexions entrantes
	result = listen(m_ListenSocket, m_MaxConcurrentConnections);
	if (result == SOCKET_ERROR) {
		cout << "listen failed with error: " << WSAGetLastError() << endl;
		FreeServer();
		return;
	}
}

//! Libère les sockets
void Server::FreeServer()
{
	closesocket(m_ListenSocket);

	if (s_InstanceCount <= 1)
		WSACleanup();
}

//! Traitement de la commande envoyée par le client
void Server::TreatCommand(string _cmd) 
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

//! Envoie de la réponse au client
void Server::Reply(string _message)
{
	if (_message.empty()) {
		cout << "Server::Reply : Message is empty." << endl;
		return;
	}
	send(m_CSocket, _message.c_str(), strlen(_message.c_str()), 0);
	cout << _message << endl;
}

//! Traitement d'un commande de volume
string Server::VolumeCommand(string _param)
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
string Server::ClassicCommand(string _param)
{
	ostringstream reply;

	if (_param == Message::HELLO_SERVER) {
		reply << Message::HELLO_CLIENT;

	// Commande de test
	} else if (_param == Message::TEST_COMMAND) {
		reply << "Code has been tested";
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

