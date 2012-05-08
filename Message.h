#pragma once
#include <string> 

using namespace std;

class Message
{

public:
	
// Code de message
	static const string CODE_AI;
	static const string CODE_KEYBOARD;
	static const string CODE_KEYBOARD_COMBO;
	static const string CODE_MEDIA;
	static const string CODE_VOLUME;
	static const string CODE_APP;
	static const string CODE_CLASSIC;
	
	// Gestion général
	static const string HELLO_SERVER;
	static const string HELLO_CLIENT;
	static const string TEST_COMMAND;
	static const string KILL_SERVER;
	static const string SHUTDOWN;
	static const string MONITOR_SWITCH_WINDOW;
	
	// Gestion des touches du clavier 
	static const string KB_ENTER;
	static const string KB_SPACE;
	static const string KB_BACKSPACE;
	static const string KB_ESCAPE;
	static const string KB_ALT_F4;
	static const string KB_CTRL_ENTER;

	// Gestion du multi-media
	static const string MEDIA_PLAY_PAUSE;
	static const string MEDIA_STOP;
	static const string MEDIA_PREVIOUS;
	static const string MEDIA_NEXT;
	
	// Gestion du son
	static const string VOLUME_MUTE;
	static const string VOLUME_UP;
	static const string VOLUME_DOWN;

	// Gestion des fenêtres et applications
	static const string APP_GOM_PLAYER;
	static const string KILL_GOM_PLAYER;
	static const string GOM_PLAYER_STRETCH;
	
	// Explorateur de fichiers
	static const string OPEN_DIR;
	static const string OPEN_FILE;

	// Intelligence Artificielle
	static const string AI_MUTE;

};

