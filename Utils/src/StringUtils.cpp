#include "StdAfx.h"
#include "StringUtils.h"

using namespace std;

/** 
 * Clear a char* buffer.
 * @param _buffer The buffer to clear.
 */
void StringUtils::ClearBuffer(char* _buffer)
{
	memset(_buffer, 0, sizeof(_buffer));
	//memset(_buffer, '\0', sizeof(_buffer));
}

bstr_t StringUtils::StringToBStr(string _str)
{
	bstr_t bstr(_str.c_str());
	return bstr;
}