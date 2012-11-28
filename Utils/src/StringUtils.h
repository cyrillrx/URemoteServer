#pragma once

#include <iostream>
#include <comdef.h>

class StringUtils
{
public:
	static void ClearBuffer(char* _buffer);
	static bstr_t StringToBStr(std::string _str);
};