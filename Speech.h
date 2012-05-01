#pragma once

#include <string>
#include <iostream>
#include <comdef.h>
#include <sphelper.h>

//#include <sapi.h>
using namespace std;

class Speech
{
private:
	static wchar_t* StrToWChar(string _textToSpeak);
	static wchar_t* CharToWChar(char *_str);
	static void SayB(bstr_t _textToSpeak);

public:
	static void SayW(LPCWSTR _textToSpeak);
	static void Say(string _textToSpeak);
	static void Say(char * _textToSpeak);

};

