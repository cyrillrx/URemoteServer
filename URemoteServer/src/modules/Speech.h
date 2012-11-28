#pragma once

#include <string>
#include <iostream>
#include <comdef.h>
#include <sphelper.h>

class Speech
{
private:
	static void SayB(bstr_t textToSpeak);
	static DWORD WINAPI SayThread(void* textToSpeak);

public:
	static void Say(std::string textToSpeak);
	static void SayInThread(std::string textToSpeak);
};