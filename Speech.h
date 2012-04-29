#pragma once

#include <string>
#include <iostream>
#include <comdef.h> // bstr_t and windows related stuff
#include <sphelper.h>
//#include <atlbase.h>
//extern CComModule _Module;
//#include <atlcom.h>

//#include <sapi.h>
using namespace std;

class Speech
{
private:
	static wchar_t* StrToWChar(string _textToSpeak);
	static wchar_t* CharToWChar(char *_str);
	static void SayW(LPCWSTR _textToSpeak);
	static void SayB(bstr_t _textToSpeak);

public:
	static void Say(string _textToSpeak);
	static void Say(char * _textToSpeak);

};

