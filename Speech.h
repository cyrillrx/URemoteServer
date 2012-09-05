#pragma once

#include <string>
#include <iostream>
#include <comdef.h>
#include <sphelper.h>

using namespace std;

class Speech
{
private:
	static void SayB(bstr_t _textToSpeak);
	static DWORD WINAPI SayThread(void* _textToSpeak);

public:
	static void Say(string _textToSpeak);
	static void SayInThread(string _textToSpeak);
};

