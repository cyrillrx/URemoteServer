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

public:
	static void Say(string _textToSpeak);

};

