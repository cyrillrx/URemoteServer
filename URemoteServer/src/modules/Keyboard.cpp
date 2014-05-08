#include "Keyboard.h"

#include "Utils.h"

#include "platform_config.h"
# if defined(WINDOWS_PLATFORM)
#	include <windows.h>
# else

#define VK_LEFT     0x25
#define VK_UP       0x26
#define VK_RIGHT    0x27
#define VK_DOWN     0x28

#define VK_MEDIA_PLAY_PAUSE 0
#define VK_MEDIA_STOP       0
#define VK_MEDIA_PREV_TRACK 0
#define VK_MEDIA_NEXT_TRACK 0

#define VK_RETURN   0
#define VK_SPACE    0
#define VK_BACK     0
#define VK_ESCAPE   0
#define VK_F4       0
#define VK_CONTROL  0
#define VK_MENU     0
#define VK_RMENU    0
#define VK_SHIFT    0
#define VK_RWIN     0

# endif // define platform

#define VK_NONE (unsigned short)-1

#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

using namespace network_io;

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

void Keyboard::HandleMessage(Response* reply, Request* request)
{
	const auto code			= request->code();
	const auto extraCode	= request->extracode();
	const auto strParam		= request->stringparam();

	const auto extraInput = GetInputFromCode(extraCode);

	switch (code) {

	case Request_Code_DEFINE:
		SendDefinedKey(strParam, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message(strParam);
		break;

	case Request_Code_LEFT:
		SendKeyboardInput(VK_LEFT, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Gauche");
		break;

	case Request_Code_RIGHT:
		SendKeyboardInput(VK_RIGHT, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Droite");
		break;

	case Request_Code_UP:
		SendKeyboardInput(VK_UP, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Haut");
		break;

	case Request_Code_DOWN:
		SendKeyboardInput(VK_DOWN, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Bas");
		break;

	case Request_Code_MEDIA_PLAY_PAUSE:
		SendKeyboardInput(VK_MEDIA_PLAY_PAUSE, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Play/pause");
		break;

	case Request_Code_MEDIA_STOP:
		SendKeyboardInput(VK_MEDIA_STOP, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Stop");
		break;

	case Request_Code_MEDIA_PREVIOUS:
		SendKeyboardInput(VK_MEDIA_PREV_TRACK, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Previous");
		break;

	case Request_Code_MEDIA_NEXT:
		SendKeyboardInput(VK_MEDIA_NEXT_TRACK, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Next");
		break;

	case Request_Code_KB_ENTER:
		SendKeyboardInput(VK_RETURN, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Enter");
		break;

	case Request_Code_KB_SPACE:
		SendKeyboardInput(VK_SPACE, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Space");
		break;

	case Request_Code_KB_BACKSPACE:
		SendKeyboardInput(VK_BACK, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Backspace");
		break;

	case Request_Code_KB_ESCAPE:
		SendKeyboardInput(VK_ESCAPE, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Escape");
		break;

	case Request_Code_KB_F4:
		SendKeyboardInput(VK_F4, extraInput);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Window closed (Alt + F4)");
		break;

	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("Unknown Keyboard code received : " + code);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////////
const unsigned short Keyboard::GetInputFromCode(Request::Code code)
{
	switch (code) {

	case Request_Code_KB_CTRL:		return VK_CONTROL;
	case Request_Code_KB_ALT:		return VK_MENU;
	case Request_Code_KB_ALTGR:		return VK_RMENU;
	case Request_Code_KB_SHIFT:		return VK_SHIFT;
	case Request_Code_KB_WINDOWS:	return VK_RWIN;
	case Request_Code_NONE:			return VK_NONE;

	default:
		return VK_NONE;
	}
}

std::string Keyboard::SendDefinedKey(const std::string& param, const unsigned short& extraCode)
{
	auto c = param.c_str()[0];
	if ((c >= 48 && c <= 57) || // 0 to 9
		(c >= 65 && c <= 90) || // A to Z
		(c >= 97 && c <= 122) ) { // a to z
			SendKeyboardInput(c, extraCode);
			return param;

	} else {
		return "Unknown command key !";
	}
}

void Keyboard::CtrlEnter()
{
	SendKeyboardInput(VK_RETURN, VK_CONTROL);
}

void Keyboard::SendKeyboardInput(const unsigned short& code, const unsigned short& extraCode)
{
	Utils::get_logger()->debug("Keyboard::SendKeyboardInput");
	Utils::get_logger()->debug("-- code : "			+ code);
	Utils::get_logger()->debug("-- extra code : "	+ extraCode);

# if defined(WINDOWS_PLATFORM)
#define KEYEVENTF_KEYDOWN 0x0000

	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	// Press the extra key
	if (extraCode != VK_NONE) {
		input.ki.wVk = extraCode;
		input.ki.dwFlags = KEYEVENTF_KEYDOWN;
		SendInput(1, &input, sizeof(INPUT));
	}

	// Press the main key
	input.ki.wVk = code;
	input.ki.dwFlags = KEYEVENTF_KEYDOWN;
	SendInput(1, &input, sizeof(INPUT));

	// Release the main key
	input.ki.wVk = code;
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));

	// Release the extra key
	if (extraCode != VK_NONE) {
		input.ki.wVk = extraCode;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}
	# else
        //TODO: Implement Keyboard::SendKeyboardInput() on Linux
	# endif

}
