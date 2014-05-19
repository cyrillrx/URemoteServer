#include "keyboard.h"

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

void keyboard::handle_message(Response* reply, Request* request)
{
	const auto code				= request->code();
	const auto modifierFlags	= request->intextra();
	const auto strExtra			= request->stringextra();

	switch (code) {

	case Request_Code_DEFINE:
		SendDefinedKey(strExtra, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message(strExtra);
		break;

	case Request_Code_DPAD_LEFT:
		SendKeyboardInput(VK_LEFT, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Gauche");
		break;

	case Request_Code_DPAD_RIGHT:
		SendKeyboardInput(VK_RIGHT, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Droite");
		break;

	case Request_Code_DPAD_UP:
		SendKeyboardInput(VK_UP, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Haut");
		break;

	case Request_Code_DPAD_DOWN:
		SendKeyboardInput(VK_DOWN, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Bas");
		break;

	case Request_Code_MEDIA_PLAY_PAUSE:
		SendKeyboardInput(VK_MEDIA_PLAY_PAUSE, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Play/pause");
		break;

	case Request_Code_MEDIA_STOP:
		SendKeyboardInput(VK_MEDIA_STOP, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Stop");
		break;

	case Request_Code_MEDIA_PREVIOUS:
		SendKeyboardInput(VK_MEDIA_PREV_TRACK, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Previous");
		break;

	case Request_Code_MEDIA_NEXT:
		SendKeyboardInput(VK_MEDIA_NEXT_TRACK, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Next");
		break;

	case Request_Code_KEYCODE_ENTER:
		SendKeyboardInput(VK_RETURN, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Enter");
		break;

	case Request_Code_KEYCODE_SPACE:
		SendKeyboardInput(VK_SPACE, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Space");
		break;

	case Request_Code_KEYCODE_BACKSPACE:
		SendKeyboardInput(VK_BACK, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Backspace");
		break;

	case Request_Code_KEYCODE_ESCAPE:
		SendKeyboardInput(VK_ESCAPE, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Escape");
		break;

	case Request_Code_KEYCODE_F4:
		SendKeyboardInput(VK_F4, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Window closed (Alt + F4)");
		break;

	case Request_Code_KEYCODE_A:
		SendKeyboardInput(VK_A, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("A");
		break;

	case Request_Code_KEYCODE_B:
		SendKeyboardInput(VK_B, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("B");
		break;

	case Request_Code_KEYCODE_C:
		SendKeyboardInput(VK_C, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("C");
		break;

	case Request_Code_KEYCODE_D:
		SendKeyboardInput(VK_D, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("D");
		break;

	case Request_Code_KEYCODE_E:
		SendKeyboardInput(VK_E, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("E");
		break;
		
	case Request_Code_KEYCODE_F:
		SendKeyboardInput(VK_F, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("F");
		break;

	case Request_Code_KEYCODE_G:
		SendKeyboardInput(VK_G, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("G");
		break;

	case Request_Code_KEYCODE_H:
		SendKeyboardInput(VK_H, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("H");
		break;

	case Request_Code_KEYCODE_I:
		SendKeyboardInput(VK_I, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("I");
		break;

	case Request_Code_KEYCODE_J:
		SendKeyboardInput(VK_J, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("J");
		break;

	case Request_Code_KEYCODE_K:
		SendKeyboardInput(VK_K, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("K");
		break;

	case Request_Code_KEYCODE_L:
		SendKeyboardInput(VK_L, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("L");
		break;

	case Request_Code_KEYCODE_M:
		SendKeyboardInput(VK_M, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("M");
		break;

	case Request_Code_KEYCODE_N:
		SendKeyboardInput(VK_N, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("N");
		break;

	case Request_Code_KEYCODE_O:
		SendKeyboardInput(VK_O, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("O");
		break;

	case Request_Code_KEYCODE_P:
		SendKeyboardInput(VK_P, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("P");
		break;

	case Request_Code_KEYCODE_Q:
		SendKeyboardInput(VK_Q, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Q");
		break;

	case Request_Code_KEYCODE_R:
		SendKeyboardInput(VK_R, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("R");
		break;

	case Request_Code_KEYCODE_S:
		SendKeyboardInput(VK_S, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("S");
		break;

	case Request_Code_KEYCODE_T:
		SendKeyboardInput(VK_T, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("T");
		break;

	case Request_Code_KEYCODE_U:
		SendKeyboardInput(VK_U, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("U");
		break;

	case Request_Code_KEYCODE_V:
		SendKeyboardInput(VK_V, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("V");
		break;

	case Request_Code_KEYCODE_W:
		SendKeyboardInput(VK_W, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("W");
		break;

	case Request_Code_KEYCODE_X:
		SendKeyboardInput(VK_X, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("X");
		break;

	case Request_Code_KEYCODE_Y:
		SendKeyboardInput(VK_Y, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Y");
		break;

	case Request_Code_KEYCODE_Z:
		SendKeyboardInput(VK_Z, modifierFlags);
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Z");
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

std::string keyboard::SendDefinedKey(const std::string& param, const int& modifierFlags)
{
	auto c = param.c_str()[0];
	if ((c >= 48 && c <= 57) || // 0 to 9
		(c >= 65 && c <= 90) || // A to Z
		(c >= 97 && c <= 122) ) { // a to z
			SendKeyboardInput(c, modifierFlags);
			return param;

	} else {
		return "Unknown command key !";
	}
}

void keyboard::CtrlEnter()
{
	SendKeyboardInput(VK_RETURN, VK_CONTROL);
}

void keyboard::SendKeyboardInput(const unsigned short& code, const int& modifierFlags)
{
	Utils::get_logger()->debug("keyboard::SendKeyboardInput");
	Utils::get_logger()->debug("-- code : "			+ code);
	Utils::get_logger()->debug("-- modifiers : " 	+ modifierFlags);

	const bool isCtrl		= FLAG_NONE != (modifierFlags & FLAG_CTRL);
	const bool isShift		= FLAG_NONE != (modifierFlags & FLAG_SHIFT);
	const bool isAltLeft	= FLAG_NONE != (modifierFlags & FLAG_ALT_LEFT);
	const bool isAltRight	= FLAG_NONE != (modifierFlags & FLAG_ALT_RIGHT);
	const bool isWindows	= FLAG_NONE != (modifierFlags & FLAG_WINDOWS);

	# if defined(WINDOWS_PLATFORM)
	#define KEYEVENTF_KEYDOWN 0x0000

		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = 0;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;

		// Press the extra keys
		if (isCtrl)		{ press_key(input, VK_CONTROL);	}
		if (isShift)	{ press_key(input, VK_SHIFT);	}
		if (isAltLeft)	{ press_key(input, VK_MENU);	}
		if (isAltRight)	{ press_key(input, VK_RMENU);	}
		if (isWindows)	{ press_key(input, VK_RWIN);	}

		// Press the main key
		press_key(input, code);

		// Release the main key
		release_key(input, code);

		// Release the extra keys
		if (isWindows)	{ release_key(input, VK_RWIN);		}
		if (isAltRight)	{ release_key(input, VK_RMENU);		}
		if (isAltLeft)	{ release_key(input, VK_MENU);		}
		if (isShift)	{ release_key(input, VK_SHIFT);		}
		if (isCtrl)		{ release_key(input, VK_CONTROL);	}
	# else
        //TODO: Implement keyboard::SendKeyboardInput() on Linux
	# endif

}

# if defined(WINDOWS_PLATFORM)
	void keyboard::press_key(INPUT &input, const unsigned short& keyCode)
	{
		input.ki.wVk = keyCode;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	void keyboard::release_key(INPUT &input, const unsigned short& keyCode)
	{
		input.ki.wVk = keyCode;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}
# endif