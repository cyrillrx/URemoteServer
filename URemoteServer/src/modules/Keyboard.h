#pragma once

#include <string>
#include "network_io/message.pb.h"

class Keyboard
{
public:
	static void HandleMessage(network_io::Response* reply, network_io::Request* request);
	static void CtrlEnter();

private :
	static const unsigned short GetInputFromCode(network_io::Request::Code code);
	static void SendKeyboardInput(const unsigned short& keyCode, const unsigned short& secondKeyCode);
	static std::string SendDefinedKey(const std::string& param, const unsigned short& extraCode);
};
