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
		SendKeyboardInput(VK_F4, extraCode);
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
		case Request_Code_KB_SHIFT:		return VK_SHIFT;
		case Request_Code_KB_WINDOWS:	return VK_RWIN;
		case Request_Code_NONE:			return -1;


		default: 
			return -1;
	}
}

string Keyboard::SendDefinedKey(string _param, WORD _extraCode)
{
	if (_param == "0") {
		SendKeyboardInput(VK_0, _extraCode);

	} else if (_param == "1") {
		SendKeyboardInput(VK_1, _extraCode);

	} else if (_param == "2") {
		SendKeyboardInput(VK_2, _extraCode);

	} else if (_param == "3") {
		SendKeyboardInput(VK_3, _extraCode);
		
	} else if (_param == "4") {
		SendKeyboardInput(VK_4, _extraCode);

	} else if (_param == "5") {
		SendKeyboardInput(VK_5, _extraCode);

	} else if (_param == "6") {
		SendKeyboardInput(VK_6, _extraCode);

	} else if (_param == "7") {
		SendKeyboardInput(VK_7, _extraCode);

	} else if (_param == "8") {
		SendKeyboardInput(VK_8, _extraCode);

	} else if (_param == "9") {
		SendKeyboardInput(VK_9, _extraCode);

	} else if (_param == "A") {
		SendKeyboardInput(VK_A, _extraCode);

	} else if (_param == "B") {
		SendKeyboardInput(VK_B, _extraCode);

	} else if (_param == "C") {
		SendKeyboardInput(VK_C, _extraCode);

	} else if (_param == "D") {
		SendKeyboardInput(VK_D, _extraCode);

	} else if (_param == "E") {
		SendKeyboardInput(VK_E, _extraCode);

	} else if (_param == "F") {
		SendKeyboardInput(VK_F, _extraCode);

	} else if (_param == "G") {
		SendKeyboardInput(VK_G, _extraCode);

	} else if (_param == "H") {
		SendKeyboardInput(VK_H, _extraCode);

	} else if (_param == "I") {
		SendKeyboardInput(VK_I, _extraCode);

	} else if (_param == "J") {
		SendKeyboardInput(VK_J, _extraCode);

	} else if (_param == "K") {
		SendKeyboardInput(VK_K, _extraCode);

	} else if (_param == "L") {
		SendKeyboardInput(VK_L, _extraCode);

	} else if (_param == "M") {
		SendKeyboardInput(VK_M, _extraCode);

	} else if (_param == "N") {
		SendKeyboardInput(VK_N, _extraCode);

	} else if (_param == "O") {
		SendKeyboardInput(VK_O, _extraCode);

	} else if (_param == "P") {
		SendKeyboardInput(VK_P, _extraCode);

	} else if (_param == "Q") {
		SendKeyboardInput(VK_Q, _extraCode);

	} else if (_param == "R") {
		SendKeyboardInput(VK_R, _extraCode);

	} else if (_param == "S") {
		SendKeyboardInput(VK_S, _extraCode);

	} else if (_param == "T") {
		SendKeyboardInput(VK_T, _extraCode);

	} else if (_param == "U") {
		SendKeyboardInput(VK_U, _extraCode);

	} else if (_param == "V") {
		SendKeyboardInput(VK_V, _extraCode);

	} else if (_param == "W") {
		SendKeyboardInput(VK_W, _extraCode);

	} else if (_param == "X") {
		SendKeyboardInput(VK_X, _extraCode);

	} else if (_param == "Y") {
		SendKeyboardInput(VK_Y, _extraCode);

	} else if (_param == "Z") {
		SendKeyboardInput(VK_Z, _extraCode);

	} else {
		return "Unknown command key !";
	}
	return _param;
}

void Keyboard::CtrlEnter()
{
	SendKeyboardInput(VK_RETURN, VK_CONTROL);
}

/*
void Keyboard::SendKeyboardInput(WORD _keyCode)
{
	KEYBDINPUT kbInput;
	kbInput.wVk = _keyCode;
	kbInput.wScan = 0;
	kbInput.dwFlags = 0;
	kbInput.time = 0;
	kbInput.dwExtraInfo = (ULONG_PTR) GetMessageExtraInfo();

	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki   = kbInput;

	SendInput(1, &input, sizeof(INPUT));
}
*/
void Keyboard::SendKeyboardInput(WORD _code, WORD _extraCode)
{
	cout << "in Keyboard::SendKeyboardInput(WORD _code, WORD _extraCode)" << endl;
	cout << "-- code : " << _code << endl;
	cout << "-- extra code : " << _extraCode << endl;

	/*
	KEYBDINPUT kbInput;
	kbInput.wVk = _keyCode;
	kbInput.wScan = 0;
	kbInput.dwFlags = 0;
	kbInput.time = 0;
	kbInput.dwExtraInfo = (ULONG_PTR) GetMessageExtraInfo();

	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki   = kbInput;

	SendInput(1, &input, sizeof(INPUT));
	*/
	INPUT combined_inputs[] =
	{
		{ INPUT_KEYBOARD, MAKELONG(_extraCode, 0) },
		{ INPUT_KEYBOARD, MAKELONG(_code, MapVirtualKey(VK_SPACE, 0)) },
		{ INPUT_KEYBOARD, MAKELONG(_code, MapVirtualKey(VK_SPACE, 0)), KEYEVENTF_KEYUP },
		{ INPUT_KEYBOARD, MAKELONG(_extraCode, 0), KEYEVENTF_KEYUP }
	};
	SendInput(ARRAYSIZE(combined_inputs), combined_inputs, sizeof INPUT);
}