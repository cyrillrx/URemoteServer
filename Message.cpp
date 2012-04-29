#include "Message.h"

// Code de message
const string  Message::CODE_KEYBOARD	= "code_keyboard";
const string  Message::CODE_KEYBOARD_COMBO	= "code_combo";
const string Message::CODE_MEDIA	= "code_media";
const string Message::CODE_VOLUME	= "code_volume";
const string Message::CODE_APP		= "code_app";
const string Message::CODE_CLASSIC	= "code_classic";

// Gestion général
const string Message::HELLO_SERVER	= "Hello serveur";
const string Message::HELLO_CLIENT	= "Hello client";
const string Message::TEST_COMMAND	= "test_command";
const string Message::KILL_SERVER	= "kill_server";
const string Message::MONITOR_SWITCH_WINDOW = "switch_window";

// Gestion des touches du clavier 
const string  Message::KB_ENTER		= "enter";
const string  Message::KB_SPACE		= "space";
const string  Message::KB_BACKSPACE	= "backspace";
const string  Message::KB_ESCAPE	= "escape";
const string  Message::KB_ALT_F4	= "alt_f4";
const string  Message::KB_CTRL_ENTER= "ctrl_enter";

// Gestion du multi-media
const string Message::MEDIA_PLAY_PAUSE	= "play_pause";
const string Message::MEDIA_STOP		= "stop";
const string Message::MEDIA_PREVIOUS	= "previous";
const string Message::MEDIA_NEXT		= "next";
	
// Gestion du son
const string Message::VOLUME_MUTE	= "volume_mute";
const string Message::VOLUME_UP		= "volume_up";
const string Message::VOLUME_DOWN	= "volume_down";

// Gestion des applications
const string Message::APP_GOM_PLAYER	= "app_gom_player";
const string Message::KILL_GOM_PLAYER	= "kill_gom_player";
const string Message::GOM_PLAYER_STRETCH	= "gom_player_stretch";

// Explorateur de fichiers
const string Message::OPEN_DIR	= "open_directory";
const string Message::OPEN_FILE	= "open_file";

