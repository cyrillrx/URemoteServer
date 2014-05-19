#pragma once

#include <string>
#include "network_io/message.pb.h"

#include "platform_config.h"
# if defined(WINDOWS_PLATFORM)
#	include <windows.h>
# else
# endif // define platform

class keyboard
{
public:
	static void handle_message(network_io::Response* reply, network_io::Request* request);
	static void CtrlEnter();

private :
	static const unsigned short GetInputFromCode(network_io::Request::Code code);
	static void SendKeyboardInput(const unsigned short& keyCode, const int& modifierKeyFlags);
	static std::string SendDefinedKey(const std::string& param, const int& modifierKeyFlags);
	
	# if defined(WINDOWS_PLATFORM)
		static void press_key(INPUT &input, const unsigned short& keyCode);
		static void release_key(INPUT &input, const unsigned short& keyCode);
	# endif

	static const int FLAG_NONE		= 0;
	static const int FLAG_CTRL		= 1;
	static const int FLAG_SHIFT		= 2;
	static const int FLAG_ALT_LEFT	= 4;
	static const int FLAG_ALT_RIGHT	= 8;
	static const int FLAG_WINDOWS	= 16;
};
