#include "keyboard.h"

#include "Utils.h"

// TODO: comments FR => EN
#include "platform_config.h"
# if defined(WINDOWS_PLATFORM)
#	include <windows.h>
# else

// http://www.kbdedit.com/manual/low_level_vk_list.html

#define VK_LEFT     0x25
#define VK_UP       0x26
#define VK_RIGHT    0x27
#define VK_DOWN     0x28

#define VK_MEDIA_NEXT_TRACK 0xB0
#define VK_MEDIA_PREV_TRACK 0xB1
#define VK_MEDIA_STOP       0xB2
#define VK_MEDIA_PLAY_PAUSE 0xB3

#define VK_RETURN	0x0D
#define VK_SPACE	0x20
#define VK_BACK     0x08
#define VK_ESCAPE   0x1B
#define VK_CONTROL  0xA2
#define VK_LCONTROL 0xA2
#define VK_RCONTROL 0xA3

#define VK_MENU     0xA4
#define VK_LMENU    0xA4
#define VK_RMENU    0xA5

#define VK_SHIFT    0xA0
#define VK_LSHIFT	0xA0
#define VK_RSHIFT   0xA1

#define VK_RWIN     0x5C

#define VK_F1	0x70
#define VK_F2	0x71
#define VK_F3	0x72
#define VK_F4	0x73
#define VK_F5	0x74
#define VK_F6	0x75
#define VK_F7	0x76
#define VK_F8	0x77
#define VK_F9	0x78
#define VK_F10	0x79
#define VK_F11	0x7A
#define VK_F12	0x7B

#define VK_OEM_PERIOD	0xBE

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

void keyboard::handle_request(const Request& request, Response& reply)
{
	const auto code				= request.code();
	const auto modifierFlags	= request.intextra();
	const auto strExtra			= request.stringextra();

	switch (code) {

	case Request_Code_DEFINE:
		SendDefinedKey(strExtra, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message(strExtra);
		break;

	case Request::DPAD_LEFT:
		SendKeyboardInput(VK_LEFT, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Gauche");
		break;

	case Request::DPAD_RIGHT:
		SendKeyboardInput(VK_RIGHT, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Droite");
		break;

	case Request::DPAD_UP:
		SendKeyboardInput(VK_UP, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Haut");
		break;

	case Request::DPAD_DOWN:
		SendKeyboardInput(VK_DOWN, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Bas");
		break;

	case Request::MEDIA_PLAY_PAUSE:
		SendKeyboardInput(VK_MEDIA_PLAY_PAUSE, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Play/pause");
		break;

	case Request::MEDIA_STOP:
		SendKeyboardInput(VK_MEDIA_STOP, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Stop");
		break;

	case Request::MEDIA_PREVIOUS:
		SendKeyboardInput(VK_MEDIA_PREV_TRACK, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Previous");
		break;

	case Request::MEDIA_NEXT:
		SendKeyboardInput(VK_MEDIA_NEXT_TRACK, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Next");
		break;

	case Request::KEYCODE_ENTER:
		SendKeyboardInput(VK_RETURN, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Enter");
		break;

	case Request::KEYCODE_SPACE:
		SendKeyboardInput(VK_SPACE, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Space");
		break;

	case Request::KEYCODE_BACKSPACE:
		SendKeyboardInput(VK_BACK, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Backspace");
		break;

	case Request::KEYCODE_ESCAPE:
		SendKeyboardInput(VK_ESCAPE, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Escape");
		break;

	//
	// Number keys
	//

	case Request::KEYCODE_0:
		SendKeyboardInput(VK_0, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("0");
		break;

	case Request::KEYCODE_1:
		SendKeyboardInput(VK_1, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("1");
		break;

	case Request::KEYCODE_2:
		SendKeyboardInput(VK_2, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("2");
		break;

	case Request::KEYCODE_3:
		SendKeyboardInput(VK_3, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("3");
		break;

	case Request::KEYCODE_4:
		SendKeyboardInput(VK_4, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("4");
		break;

	case Request::KEYCODE_5:
		SendKeyboardInput(VK_5, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("5");
		break;

	case Request::KEYCODE_6:
		SendKeyboardInput(VK_6, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("6");
		break;

	case Request::KEYCODE_7:
		SendKeyboardInput(VK_7, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("7");
		break;

	case Request::KEYCODE_8:
		SendKeyboardInput(VK_8, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("8");
		break;

	case Request::KEYCODE_9:
		SendKeyboardInput(VK_9, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("9");
		break;
		
	//
	// Letter keys (A-Z)
	//

	case Request::KEYCODE_A:
		SendKeyboardInput(VK_A, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("A");
		break;

	case Request::KEYCODE_B:
		SendKeyboardInput(VK_B, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("B");
		break;

	case Request::KEYCODE_C:
		SendKeyboardInput(VK_C, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("C");
		break;

	case Request::KEYCODE_D:
		SendKeyboardInput(VK_D, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("D");
		break;

	case Request::KEYCODE_E:
		SendKeyboardInput(VK_E, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("E");
		break;
		
	case Request::KEYCODE_F:
		SendKeyboardInput(VK_F, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F");
		break;

	case Request::KEYCODE_G:
		SendKeyboardInput(VK_G, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("G");
		break;

	case Request::KEYCODE_H:
		SendKeyboardInput(VK_H, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("H");
		break;

	case Request::KEYCODE_I:
		SendKeyboardInput(VK_I, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("I");
		break;

	case Request::KEYCODE_J:
		SendKeyboardInput(VK_J, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("J");
		break;

	case Request::KEYCODE_K:
		SendKeyboardInput(VK_K, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("K");
		break;

	case Request::KEYCODE_L:
		SendKeyboardInput(VK_L, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("L");
		break;

	case Request::KEYCODE_M:
		SendKeyboardInput(VK_M, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("M");
		break;

	case Request::KEYCODE_N:
		SendKeyboardInput(VK_N, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("N");
		break;

	case Request::KEYCODE_O:
		SendKeyboardInput(VK_O, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("O");
		break;

	case Request::KEYCODE_P:
		SendKeyboardInput(VK_P, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("P");
		break;

	case Request::KEYCODE_Q:
		SendKeyboardInput(VK_Q, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Q");
		break;

	case Request::KEYCODE_R:
		SendKeyboardInput(VK_R, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("R");
		break;

	case Request::KEYCODE_S:
		SendKeyboardInput(VK_S, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("S");
		break;

	case Request::KEYCODE_T:
		SendKeyboardInput(VK_T, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("T");
		break;

	case Request::KEYCODE_U:
		SendKeyboardInput(VK_U, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("U");
		break;

	case Request::KEYCODE_V:
		SendKeyboardInput(VK_V, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("V");
		break;

	case Request::KEYCODE_W:
		SendKeyboardInput(VK_W, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("W");
		break;

	case Request::KEYCODE_X:
		SendKeyboardInput(VK_X, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("X");
		break;

	case Request::KEYCODE_Y:
		SendKeyboardInput(VK_Y, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Y");
		break;

	case Request::KEYCODE_Z:
		SendKeyboardInput(VK_Z, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Z");
		break;

	//
	// Special F1 - F12
	//

	case Request::KEYCODE_F1:
		SendKeyboardInput(VK_F1, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F1");
		break;

	case Request::KEYCODE_F2:
		SendKeyboardInput(VK_F2, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F2");
		break;

	case Request::KEYCODE_F3:
		SendKeyboardInput(VK_F3, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F3");
		break;

	case Request::KEYCODE_F4:
		SendKeyboardInput(VK_F4, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F4");
		break;

	case Request::KEYCODE_F5:
		SendKeyboardInput(VK_F5, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F5");
		break;

	case Request::KEYCODE_F6:
		SendKeyboardInput(VK_F6, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F6");
		break;

	case Request::KEYCODE_F7:
		SendKeyboardInput(VK_F7, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F7");
		break;

	case Request::KEYCODE_F8:
		SendKeyboardInput(VK_F8, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F8");
		break;

	case Request::KEYCODE_F9:
		SendKeyboardInput(VK_F9, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F9");
		break;

	case Request::KEYCODE_F10:
		SendKeyboardInput(VK_F10, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F10");
		break;

	case Request::KEYCODE_F11:
		SendKeyboardInput(VK_F11, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F11");
		break;

	case Request::KEYCODE_F12:
		SendKeyboardInput(VK_F12, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("F12");
		break;

	//
	// Special char keys
	// '=', '-', '+', '*',
	// '/', '\', '_', '|'
	// ',', '.', ':', ';', '@', '''
	//
	case Request::KEYCODE_PERIODE:
		SendKeyboardInput(VK_OEM_PERIOD, modifierFlags);
		reply.set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply.set_message("Periode char '.'");
		break;

	// Default => Unhandled key

	default:
		const std::string mesasge = "Key not handled : " + Request::Code_Name(code);
		Utils::get_logger()->warning(mesasge);
		reply.set_returncode(Response_ReturnCode_RC_ERROR);
		reply.set_message(mesasge);
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
	Utils::get_logger()->debug("-- code : "			+ std::to_string(code));
	Utils::get_logger()->debug("-- modifiers : " 	+ std::to_string(modifierFlags));

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
		input.ki.dwFlags = KEYEVENTF_KEYDOWN;
		SendInput(1, &input, sizeof(INPUT));
	}

	void keyboard::release_key(INPUT &input, const unsigned short& keyCode)
	{
		input.ki.wVk = keyCode;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}
# endif