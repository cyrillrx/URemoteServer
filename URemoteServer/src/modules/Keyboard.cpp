#include "Keyboard.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

void Keyboard::HandleMessage(Response* _reply, Request* _request)
{
	const Request_Code code			= _request->code();
	const Request_Code extraCode	= _request->extracode();
	const string strParam			= _request->stringparam();
	
	string message = "";
	const WORD extraInput = GetInputFromCode(extraCode);

	switch (code) {

	case Request_Code_DEFINE:
		SendDefinedKey(strParam, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message(strParam);
		break;
		
	case Request_Code_LEFT:
		SendKeyboardInput(VK_LEFT, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Gauche");
		break;

	case Request_Code_RIGHT:
		SendKeyboardInput(VK_RIGHT, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Droite");
		break;

	case Request_Code_UP:
		SendKeyboardInput(VK_UP, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Haut");
		break;

	case Request_Code_DOWN:
		SendKeyboardInput(VK_DOWN, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Bas");
		break;

	case Request_Code_MEDIA_PLAY_PAUSE:
		SendKeyboardInput(VK_MEDIA_PLAY_PAUSE, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Play/pause");
		break;
		
	case Request_Code_MEDIA_STOP:
		SendKeyboardInput(VK_MEDIA_STOP, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Stop");
		break;

	case Request_Code_MEDIA_PREVIOUS:
		SendKeyboardInput(VK_MEDIA_PREV_TRACK, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Previous");
		break;

	case Request_Code_MEDIA_NEXT:
		SendKeyboardInput(VK_MEDIA_NEXT_TRACK, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Next");
		break;

	case Request_Code_KB_RETURN:
		SendKeyboardInput(VK_RETURN, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Enter");
		break;
		
	case Request_Code_KB_SPACE:
		SendKeyboardInput(VK_SPACE, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Space");
		break;
		
	case Request_Code_KB_BACKSPACE:
		SendKeyboardInput(VK_BACK, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Backspace");
		break;
		
	case Request_Code_KB_ESCAPE:
		SendKeyboardInput(VK_ESCAPE, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Escape");
		break;

	case Request_Code_KB_F4:
		SendKeyboardInput(VK_F4, extraInput);
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Window closed (Alt + F4)");
		break;

	default:
		_reply->set_returncode(Response_ReturnCode_RC_ERROR);
		_reply->set_message("Unknown Keyboard code received : " + code);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////////////
WORD Keyboard::GetInputFromCode(Request_Code _code)
{
	switch (_code) {
		
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

string Keyboard::SendDefinedKey(string _param, WORD _extraCode)
{
	char c = _param.c_str()[0];
	if (	(c >= 48 && c <= 57) || // 0 to 9
			(c >= 65 && c <= 90) || // A to Z
			(c >= 97 && c <= 122) ) { // a to z
		SendKeyboardInput(c, _extraCode);
		return _param;

	} else {
		return "Unknown command key !";
	}
}

void Keyboard::CtrlEnter()
{
	SendKeyboardInput(VK_RETURN, VK_CONTROL);
	SendKeyboardInput(VK_L, VK_LWIN);
}

void Keyboard::SendKeyboardInput(WORD _code, WORD _extraCode)
{
	cout << "in Keyboard::SendKeyboardInput(WORD _code, WORD _extraCode)" << endl;
	cout << "-- code : " << _code << endl;
	cout << "-- extra code : " << _extraCode << endl;
	
	#define KEYEVENTF_KEYDOWN 0x0000

	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	
	// Press the extra key
	if (_extraCode != VK_NONE) {
		input.ki.wVk = _extraCode;
		input.ki.dwFlags = KEYEVENTF_KEYDOWN;
		SendInput(1, &input, sizeof(INPUT));
	}
	
	// Press the main key
	input.ki.wVk = _code;
	input.ki.dwFlags = KEYEVENTF_KEYDOWN;
	SendInput(1, &input, sizeof(INPUT));
	
	// Release the main key
	input.ki.wVk = _code;
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
	
	// Release the extra key
	if (_extraCode != VK_NONE) {
		input.ki.wVk = _extraCode;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}
}