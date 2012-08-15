#include "Server.h"

#include <sstream>
#include <iostream>
#include <comdef.h>

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
	
	//m_ArtificialIntelligence = new AI("Synaps");
	//m_ArtificialIntelligence = new AI("Ubiquity");
	//m_ArtificialIntelligence = new AI("Arthemis");
	//m_ArtificialIntelligence = new AI("Athena");
	m_ArtificialIntelligence = new AI("Eternity");
}

Server::~Server(void)
{
	delete(m_ArtificialIntelligence);
	FreeServer();
}

//! Lancement du serveur
void Server::Launch()
{
	m_ContinueToListen = true;

	// Socket d'acceptation
	//SOCKADDR_IN csin;
	//int sizeofcsin = sizeof(csin);
	char buffer[BUFSIZ];

	/* connection socket */
	
	while (m_ContinueToListen) {
		cout << "Open port : " << m_Port << endl;
		cout << "Waiting for client to connect..." << endl;
		
		//m_CSocket = accept(m_ListenSocket, (SOCKADDR *)&csin, &sizeofcsin);
		m_CSocket = accept(m_ListenSocket, NULL, NULL);
		if (m_CSocket == INVALID_SOCKET) {
			cout << "accept failed with error: " << WSAGetLastError() << endl;
			FreeServer();
			return;
		}
		
			memset(buffer, '\0', sizeof(buffer)); // On vide le buffer
			//memset(buffer, '\0', BUFSIZ); // On vide le buffer
			int res = recv(m_CSocket, buffer, sizeof(buffer), 0);
			//int res = recv(m_CSocket, buffer, BUFSIZ, 0);
			cout << "  -- result" << res <<  endl;
			//int getpeername(int sockfd, struct sockaddr *addr, int *addrlen);
			//int gethostname(char *hostname, size_t size);
			cout << buffer << endl;
			string message = buffer;
			HandleMessage(message);
		

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
	socketAddress.sin_addr.s_addr	= htonl(INADDR_ANY); // Adresse du serveur
	socketAddress.sin_family		= AF_INET; // famille de socket utilisée => Internet
	socketAddress.sin_port			= htons(m_Port);

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
void Server::HandleMessage(string _msg) 
{
	cout << "Server::HandleMessage() : Command <" << _msg << ">" << endl;
	size_t position = _msg.find ('|');

	string code, param;
	if (position != string::npos) { // Si on trouve un séparateur
		code = _msg.substr(0, position);
		param = _msg.substr(position + 1);
	} else {
		code = _msg;
		param = "";
	}
	cout << "	Code	<" << code << ">" << endl;
	cout << "	Param	<" << param << ">" << endl;

	ostringstream reply;
	
	// Commande à l'intelligence artificielle
	if (code == Message::CODE_AI) {
		Reply(AICommand(param));
		return;
		
	} else if (code == Message::CODE_CLASSIC) {
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
		string data;
		FileManager::GetDirectoryContent(param)->SerializeToString(&data);
		reply << data;

	// Ouvrir un fichier
	} else if (code == Message::OPEN_FILE) { 
		reply << FileManager::OpenFile(param);
		
	} else if (_msg == "\0") {
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

string Server::ShutdownPC()
{
	m_ContinueToListen = false;
	//ShellExecute(NULL, L"shutdown", NULL, L"-s -t 10", NULL, SW_SHOWMAXIMIZED);
	system("Shutdown.exe -s -t 10 -c \"L'ordinateur va s'éteindre dans 10 secondes\"");
	string reply = "PC will shutdown in 10 seconds";
	m_ArtificialIntelligence->Say(reply);
	return reply;
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
	m_ArtificialIntelligence->Say(replyStr);
	return replyStr;
}

//! Traitement d'une commande général
string Server::AICommand(string _param)
{
	ostringstream reply;

	if (_param == Message::AI_MUTE) {
		bool isMute = m_ArtificialIntelligence->ToggleMute();
			
		if (isMute) {
			reply << "AI volume is off.";
		} else {
			reply << "AI volume is now on.";
			m_ArtificialIntelligence->Say(reply.str());
		}

	} else {
		return "Unknown AI command !";
	}
	return reply.str();
}

//! Traitement d'une commande général
string Server::ClassicCommand(string _param)
{
	ostringstream reply;

	if (_param == Message::HELLO_SERVER) {
		reply << Message::HELLO_CLIENT;
		m_ArtificialIntelligence->Welcome();

	// Commande de test
	} else if (_param == Message::TEST_COMMAND) {
		/*
		bool isMute = m_ArtificialIntelligence->ToggleMute();
			
		if (isMute) {
			reply << "AI volume is off.";
		} else {
			reply << "AI volume is now on.";
			m_ArtificialIntelligence->Say(reply.str());
		}
		*/

		//reply << "The code has been tested";
		reply << "No way. Go Fuck yourself !";
		m_ArtificialIntelligence->Say(reply.str());

	// Commande pour tuer le serveur
	} else if (_param == Message::KILL_SERVER) {
		m_ContinueToListen = false;
		reply << "Server killed";
		
	// Eteindre l'ordinateur
	} else if (_param == Message::SHUTDOWN) {
		reply << ShutdownPC();

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
		reply << App::GetGomPlayer()->Show();
		App::FreeGomPlayer();
		
	// Fermer Gom player
	} else if (_param == Message::KILL_GOM_PLAYER) {
		reply << App::GetGomPlayer()->Close();
		App::FreeGomPlayer();

	// Stretch Gom player
	} else if (_param == Message::GOM_PLAYER_STRETCH) {
		reply << App::GetGomPlayer()->Strech();
		App::FreeGomPlayer();
			
	} else {
		return "Unknown app command !";
	}
	
	m_ArtificialIntelligence->Say(reply.str().c_str());
	return reply.str();
}

