#pragma once

#include <iostream>
#include <comdef.h>

using namespace std;

class StringUtils
{
public:
	static void ClearBuffer(char* _buffer);
	static bstr_t StringToBStr(string _str);
};