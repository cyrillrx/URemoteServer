#include "Keyboard.h"

#include "Utils.h"

using namespace network;

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

void Keyboard::HandleMessage(Response* reply, Request* request)
{
	const Request_Code code			= request->code();
	const Request_Code extraCode	= request->extracode();
	const std::string strParam			= request->stringparam();

	const WORD extraInput = GetInputFromCode(extraCode);

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

	case Request_Code_KB_RETURN:
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
WORD Keyboard::GetInputFromCode(Request_Code code)
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

std::string Keyboard::SendDefinedKey(std::string param, WORD extraCode)
{
	char c = param.c_str()[0];
	if (	(c >= 48 && c <= 57) || // 0 to 9
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
	SendKeyboardInput(VK_L, VK_LWIN);
}

void Keyboard::SendKeyboardInput(WORD code, WORD extraCode)
{
	Utils::getLogger()->debug("Keyboard::SendKeyboardInput");
	Utils::getLogger()->debug("-- code : "			+ code);
	Utils::getLogger()->debug("-- extra code : "	+ extraCode);

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
}