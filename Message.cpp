#include "Message.h"

const string Message::TEST_COMMAND	= "test_command";

const string Message::HELLO_SERVER	= "Hello serveur";
const string Message::HELLO_CLIENT	= "Hello client";
const string Message::KILL_SERVER	= "kill_server";

// Gestion des touches du clavier 
const string  Message::KEYBOARD_SPACE	= "keyboard_space";
const string  Message::KEYBOARD_ENTER	= "keyboard_enter";
const string  Message::KEYBOARD_ALTF4	= "keyboard_altf4";

// Gestion du multi-media
const string Message::MEDIA_PLAY_PAUSE	= "media_play_pause";
const string Message::MEDIA_STOP		= "media_stop";
const string Message::MEDIA_PREVIOUS	= "media_previous";
const string Message::MEDIA_NEXT		= "media_next";
	
// Gestion du son
const string Message::VOLUME_MUTE	= "volume_mute";
const string Message::VOLUME_UP		= "volume_up";
const string Message::VOLUME_DOWN	= "volume_down";

// Gestion des fenêtres et applications
const string Message::MONITOR_SWITCH_WINDOW = "switch_window";

const string Message::KILL_APP			= "kill_app";
const string Message::APP_GOM_PLAYER	= "app_gom_player";
const string Message::KILL_GOM_PLAYER	= "kill_gom_player";
const string Message::GOM_PLAYER_STRETCH	= "gom_player_stretch";

// Explorateur de fichiers
const string Message::OPEN_DIR	= "open_directory";
const string Message::OPEN_FILE	= "open_file";


Message::Message(string _code, string _param)
{
	mCode		= _code;
	mParam		= _param;
}

Message::~Message(void)
{
}

string Message::GetCode()
{
	return mCode;
}

string Message::GetParam()
{
	return mParam;
}