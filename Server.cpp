#include "Server.h"

#include <sstream>
#include <iostream>

#include "Message.h"
#include "MasterVolume.h"
#include "Keyboard.h"
#include "App.h"
#include "FileManager.h"
#include "MonUtils.h"

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

		memset(buffer, '\0', sizeof(buffer)); // On vide le buffer
		recv(m_CSocket, buffer, sizeof(buffer), 0);
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

	string command, param;
	if (position != string::npos) { // Si on trouve un séparateur
		command = _cmd.substr(0, position);
		param = _cmd.substr(position + 1);
	} else {
		command = _cmd;
		param = "";
	}
	cout << "Code is " << command << endl;


	ostringstream reply;
	if (_cmd == Message::HELLO_SERVER) {
		reply << Message::HELLO_CLIENT;

	// Commande de test
	} else if (command == Message::TEST_COMMAND) {
		MonUtils::SwitchWindow();
		reply << "Code has been tested";
		
	// Commande pour tuer le serveur
	} else if (command == Message::KILL_SERVER) {
		m_ContinueToListen = false;
		reply << "Server killed";
		
	// Commande pour fermer la fenêtre courante
	} else if (command == Message::KEYBOARD_ALTF4) {
		Keyboard::AltF4();
		reply << "Window closed (Alt + F4)";

	// Play/Pause
	} else if (command == Message::MEDIA_PLAY_PAUSE) {
		Keyboard::PlayPause();
		reply << "Play/pause";
			
	// Stop
	} else if (command == Message::MEDIA_STOP) {
		Keyboard::Stop();
		reply << "Stop";

	// Précédent
	} else if (command == Message::MEDIA_PREVIOUS) {
		Keyboard::Previous();
		reply << "Previous";
			
	// Suivant
	} else if (command == Message::MEDIA_NEXT) {
		Keyboard::Next();
		reply << "Next";
			
	// Espace
	} else if (command == Message::KEYBOARD_SPACE) {
		Keyboard::Space();
		reply << "Space";
			
	// Enter
	} else if (command == Message::KEYBOARD_ENTER) {
		Keyboard::Enter();
		reply << "Enter";
			
	// Switch écran
	} else if (command == Message::MONITOR_SWITCH_WINDOW) {
		MonUtils::SwitchWindow();
		reply << "Switch window";
			
	// Volume +
	} else if (command == Message::VOLUME_UP) {
		MasterVolume *volume = new MasterVolume();
		float fVolumeLvl = volume->TurnUp();
		delete(volume);
		volume = NULL;

		//sprintf_s(buffer, "Volume is %f", fVolumeLvl);
		reply << "Volume up to " << fVolumeLvl * 100 << "%";
			
	// Volume -
	} else if (command == Message::VOLUME_DOWN) {
		MasterVolume *volume = new MasterVolume();
		float fVolumeLvl = volume->TurnDown();
		delete(volume);
		volume = NULL;

		//sprintf_s(buffer, "Volume is %f", fVolumeLvl);
		reply << "Volume down to " << fVolumeLvl * 100 << "%";
			
	// Volume Mute
	} else if (command == Message::VOLUME_MUTE) {
		MasterVolume *volume = new MasterVolume();
		bool isMute = volume->ToggleMute();
		delete(volume);
		volume = NULL;
			
		reply << "Mute : ";
		if (isMute) {
			reply << "Volume is off";
		} else {
			reply << "Volume is on";
		}
		
	// Fermer l'application en cours
	} else if (command == Message::KILL_APP) {
		
		reply << "Kill command launched";
	
	// Ouvrir Gom player
	} else if (command == Message::APP_GOM_PLAYER) {
		cout << Message::APP_GOM_PLAYER << endl;
		App* gomPlayer = new App("GomPlayer", "GomPlayer1.x", "C:\\Program Files (x86)\\GRETECH\\GomPlayer\\GOM.exe");
		string resultMessage = gomPlayer->Show();
		delete gomPlayer;
		gomPlayer = NULL;
		reply << resultMessage;
		
	// Fermer Gom player
	} else if (command == Message::KILL_GOM_PLAYER) {
		App* gomPlayer = new App("GomPlayer", "GomPlayer1.x", "C:\\Program Files (x86)\\GRETECH\\GomPlayer\\GOM.exe");
		string resultMessage = gomPlayer->Close();
		delete gomPlayer;
		gomPlayer = NULL;
		reply << resultMessage;

	// Stretch Gom player
	} else if (command == Message::GOM_PLAYER_STRETCH) {
		App* gomPlayer = new App("GomPlayer", "GomPlayer1.x", "C:\\Program Files (x86)\\GRETECH\\GomPlayer\\GOM.exe");
		string resultMessage = gomPlayer->Strech();
		delete gomPlayer;
		gomPlayer = NULL;
		reply << resultMessage;

	// Ouvrir un dossier
	} else if (command == Message::OPEN_DIR) {
		reply << FileManager::ListFilesStr(param);
		//reply << "Ouverture du dossier : " << param;

	// Ouvrir un fichier
	} else if (command == Message::OPEN_FILE) { 
		ShellExecuteA(NULL, NULL, param.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
		reply << "Ouverture du fichier : " << param;

	} else if (_cmd == "\0") {
		reply << "No command had been receive";

	} else {
		reply << "No command had been received (else)";
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


