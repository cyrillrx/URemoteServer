#pragma once

#include <windows.h>

class Keyboard
{

private :
	static void SendKeyboardInput(WORD _keyCode);

public:
	static void PlayPause();
	static void Stop();
	static void Previous();
	static void Next();
	
	static void Space();
	static void Enter();
	static void CtrlEnter();
	static void AltF4();
};

