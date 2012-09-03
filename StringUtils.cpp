#include "StringUtils.h"

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

/*
void Speech::Say(char* _textToSpeak)
{
	cout << "- char* _textToSpeak" << endl;
	LPCWSTR str = CharToWChar(_textToSpeak);
	cout << str << endl;
	wcout << str << endl;
	SayW(str);
}

wchar_t* Speech::CharToWChar(char *_str)
{
    // Convert to a wchar_t*
    size_t origsize = strlen(_str) + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs_s(&convertedChars, wcstring, origsize, _str, _TRUNCATE);

	return wcstring;
}

wchar_t* Speech::StrToWChar(string _str)
{
	// Convert to a char*
    const size_t newsize = 100;
    char nstring[newsize];
    strcpy_s(nstring, _str.c_str());
	
	 // Convert to a wchar_t*
    // You must first convert to a char * for this to work.
    size_t origsize = strlen(_str.c_str()) + 1;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs_s(&convertedChars, wcstring, origsize, _str.c_str(), _TRUNCATE);

	return wcstring;
}
*/