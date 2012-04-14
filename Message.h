#pragma once
#include <string> 

using namespace std;

class Message
{
private:
	string mCode;
	string mParam;

public:
	static const string TEST_COMMAND;

	static const string HELLO_SERVER;
	static const string HELLO_CLIENT;
	static const string KILL_SERVER;
	
	// Gestion des touches du clavier 
	static const string KEYBOARD_SPACE;
	static const string KEYBOARD_ENTER;
	static const string KEYBOARD_ALTF4;

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
	static const string MONITOR_SWITCH_WINDOW;
	static const string KILL_APP;
	static const string APP_GOM_PLAYER;
	static const string KILL_GOM_PLAYER;
	static const string GOM_PLAYER_STRETCH;
	
	// Explorateur de fichiers
	static const string OPEN_DIR;
	static const string OPEN_FILE;

	Message(string _code, string _param);
	~Message(void);

	string GetCode();
	string GetParam();

};

