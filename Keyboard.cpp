#include "Keyboard.h"
#include "Message.h"

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

string Keyboard::Command(string _param)
{
	if (_param == Message::KB_ENTER) {
		SendKeyboardInput(VK_RETURN);

	} else if (_param == Message::KB_SPACE) {
		SendKeyboardInput(VK_SPACE);

	} else if (_param == Message::KB_BACKSPACE) {
		SendKeyboardInput(VK_BACK);

	} else if (_param == Message::KB_ESCAPE) {
		SendKeyboardInput(VK_ESCAPE);
		
	} else if (_param == "0") {
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

string Keyboard::MediaCommand(string _param)
{
	if (_param == Message::MEDIA_PLAY_PAUSE) {
		SendKeyboardInput(VK_MEDIA_PLAY_PAUSE);
		return "Play/pause";
		
	} else if (_param == Message::MEDIA_STOP) {
		SendKeyboardInput(VK_MEDIA_STOP);
		return "Stop";

	} else if (_param == Message::MEDIA_PREVIOUS) {
		SendKeyboardInput(VK_MEDIA_PREV_TRACK);
		return "Previous";

	} else if (_param == Message::MEDIA_NEXT) {
		SendKeyboardInput(VK_MEDIA_NEXT_TRACK);
		return "Next";

	} else {
		return "Unknown media key !";
	}
}

string Keyboard::Combo(string _param)
{
	if (_param == Message::KB_ALT_F4) {
		AltF4();
		return "Window closed (Alt + F4)";
		
	} else if (_param == Message::KB_CTRL_ENTER) {
		CtrlEnter();
		return "Ctrl + Enter";

	} else {
		return "Unknown combo key !";
	}
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

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