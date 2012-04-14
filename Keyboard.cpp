#include "Keyboard.h"

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

void Keyboard::PlayPause()
{
	SendKeyboardInput(VK_MEDIA_PLAY_PAUSE);
}

void Keyboard::Stop()
{
	SendKeyboardInput(VK_MEDIA_STOP);
}

void Keyboard::Previous()
{
	SendKeyboardInput(VK_MEDIA_PREV_TRACK);
}

void Keyboard::Next()
{
	SendKeyboardInput(VK_MEDIA_NEXT_TRACK);
}

void Keyboard::Space()
{
	SendKeyboardInput(VK_SPACE);
}

void Keyboard::Enter()
{
	SendKeyboardInput(VK_RETURN);
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

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

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