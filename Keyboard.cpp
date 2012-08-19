#include "Keyboard.h"

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

Response* Keyboard::HandleMessage(Request_Code _code, string _param)
{
	Response *reply = new Response();
	reply->set_returncode(Response_ReturnCode_RC_SUCCESS);

	switch (_code) {

	case Request_Code_DEFINE:
		SendKey(_param);
		reply->set_message(_param);
		break;

	case Request_Code_MEDIA_PLAY_PAUSE:
		SendKeyboardInput(VK_MEDIA_PLAY_PAUSE);
		reply->set_message("Play/pause");
		break;
		
	case Request_Code_MEDIA_STOP:
		SendKeyboardInput(VK_MEDIA_STOP);
		reply->set_message("Stop");
		break;

	case Request_Code_MEDIA_PREVIOUS:
		SendKeyboardInput(VK_MEDIA_PREV_TRACK);
		reply->set_message("Previous");
		break;

	case Request_Code_MEDIA_NEXT:
		SendKeyboardInput(VK_MEDIA_NEXT_TRACK);
		reply->set_message("Next");
		break;

	case Request_Code_KB_RETURN:
		SendKeyboardInput(VK_RETURN);
		reply->set_message("Enter");
		break;
		
	case Request_Code_KB_SPACE:
		SendKeyboardInput(VK_SPACE);
		reply->set_message("Space");
		break;
		
	case Request_Code_KB_BACKSPACE:
		SendKeyboardInput(VK_BACK);
		reply->set_message("Backspace");
		break;
		
	case Request_Code_KB_ESCAPE:
		SendKeyboardInput(VK_ESCAPE);
		reply->set_message("Escape");
		break;

	case Request_Code_KB_ALT_F4:
		AltF4();
		reply->set_message("Window closed (Alt + F4)");
		break;
		
	case Request_Code_KB_CTRL_RETURN:
		CtrlEnter();
		reply->set_message("Ctrl + Enter");
		break;

	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("Unknown Keyboard code received : " + _code);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

string Keyboard::SendKey(string _param)
{
	if (_param == "0") {
		SendKeyboardInput(VK_0);

	} else if (_param == "1") {
		SendKeyboardInput(VK_1);

	} else if (_param == "2") {
		SendKeyboardInput(VK_2);

	} else if (_param == "3") {
		SendKeyboardInput(VK_3);
		
	} else if (_param == "4") {
		SendKeyboardInput(VK_4);

	} else if (_param == "5") {
		SendKeyboardInput(VK_5);

	} else if (_param == "6") {
		SendKeyboardInput(VK_6);

	} else if (_param == "7") {
		SendKeyboardInput(VK_7);

	} else if (_param == "8") {
		SendKeyboardInput(VK_8);

	} else if (_param == "9") {
		SendKeyboardInput(VK_9);

	} else if (_param == "A") {
		SendKeyboardInput(VK_A);

	} else if (_param == "B") {
		SendKeyboardInput(VK_B);

	} else if (_param == "C") {
		SendKeyboardInput(VK_C);

	} else if (_param == "D") {
		SendKeyboardInput(VK_D);

	} else if (_param == "E") {
		SendKeyboardInput(VK_E);

	} else if (_param == "F") {
		SendKeyboardInput(VK_F);

	} else if (_param == "G") {
		SendKeyboardInput(VK_G);

	} else if (_param == "H") {
		SendKeyboardInput(VK_H);

	} else if (_param == "I") {
		SendKeyboardInput(VK_I);

	} else if (_param == "J") {
		SendKeyboardInput(VK_J);

	} else if (_param == "K") {
		SendKeyboardInput(VK_K);

	} else if (_param == "L") {
		SendKeyboardInput(VK_L);

	} else if (_param == "M") {
		SendKeyboardInput(VK_M);

	} else if (_param == "N") {
		SendKeyboardInput(VK_N);

	} else if (_param == "O") {
		SendKeyboardInput(VK_O);

	} else if (_param == "P") {
		SendKeyboardInput(VK_P);

	} else if (_param == "Q") {
		SendKeyboardInput(VK_Q);

	} else if (_param == "R") {
		SendKeyboardInput(VK_R);

	} else if (_param == "S") {
		SendKeyboardInput(VK_S);

	} else if (_param == "T") {
		SendKeyboardInput(VK_T);

	} else if (_param == "U") {
		SendKeyboardInput(VK_U);

	} else if (_param == "V") {
		SendKeyboardInput(VK_V);

	} else if (_param == "W") {
		SendKeyboardInput(VK_W);

	} else if (_param == "X") {
		SendKeyboardInput(VK_X);

	} else if (_param == "Y") {
		SendKeyboardInput(VK_Y);

	} else if (_param == "Z") {
		SendKeyboardInput(VK_Z);

	} else {
		return "Unknown command key !";
	}
	return _param;
}

void Keyboard::CtrlEnter()
{
    keybd_event(VK_CONTROL,	0, 0, 0);
    keybd_event(VK_RETURN,	0, 0, 0);
    keybd_event(VK_RETURN,	0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL,	0, KEYEVENTF_KEYUP, 0);
}

void Keyboard::AltF4()
{
    keybd_event(VK_LMENU,	0, 0, 0);
	keybd_event(VK_F4,	0, 0, 0);
    keybd_event(VK_F4,	0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_LMENU,	0, KEYEVENTF_KEYUP, 0);
}

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