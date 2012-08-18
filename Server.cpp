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

	Request request;
	request.ParseFromString(_msg);

	Request_Type requestType	= request.type();
	Request_Code requestCode	= request.code();
	string param	= request.text();
	
	wcout << "	Type	<" << request.Type_Name << ">" << endl;
	wcout << "	Code	<" << request.Code_Name << ">" << endl;
	cout << "	Param	<" << param << ">" << endl;

	ostringstream reply;
	
	switch (requestType) {
	
	case Request_Type_SIMPLE:
		Reply(ClassicCommand(requestCode));
		break;

	case Request_Type_EXPLORER:
		if (requestCode == Request_Code_OPEN_DIR) {
			string data;
			FileManager::GetDirectoryContent(param)->SerializeToString(&data);
			reply << data;

		// Ouvrir un fichier
		} else if (requestCode == Request_Code_OPEN_FILE) { 
			reply << FileManager::OpenFile(param);
		}
		break;
		
	case Request_Type_KEYBOARD:
		Reply(Keyboard::Command(param));
		Reply(Keyboard::Combo(param));
		break;

	case Request_Type_MEDIA:
		Reply(Keyboard::MediaCommand(param));
		break;

	case Request_Type_AI:
		Reply(AICommand(requestCode));
		break;
		
	case Request_Type_VOLUME:
		Reply(VolumeCommand(requestCode));
		break;

	// Commande des applications
	case Request_Type_APP:
		Reply(AppCommand(param));
		break;

	default:
		if (_msg == "\0") {
			reply << "No command had been receive";
		} else {
			reply << "Unknown code received !";
		}
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
string Server::VolumeCommand(Request_Code _code)
{
	ostringstream reply;
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

//! Traitement d'une commande général
string Server::AICommand(Request_Code _code)
{
	ostringstream reply;
	bool isMute;

	switch (_code) {

	case Request_Code_MUTE:
		isMute = m_ArtificialIntelligence->ToggleMute();
			
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

//! Traitement d'une commande général
string Server::ClassicCommand(Request_Code _code)
{
	ostringstream reply;

	switch (_code) {

	case Request_Code_HELLO:
		reply << Message::HELLO_CLIENT;
		m_ArtificialIntelligence->Welcome();
		break;
		
	case Request_Code_TEST:
		//reply << "The code has been tested";
		reply << "No way. Go Fuck yourself !";
		m_ArtificialIntelligence->Say(reply.str());
		break;

	case Request_Code_KILL_SERVER:
		m_ContinueToListen = false;
		reply << "Server killed";
		break;

	case Request_Code_SHUTDOWN:
		reply << ShutdownPC();
		break;

	case Request_Code_SWITCH_WINDOW:
		MonUtils::SwitchWindow();
		reply << "Switch window";
		break;
			
	default:
		reply <<  "Unknown classic command !";
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

