#include "Speech.h"

void Speech::SayW(LPCWSTR _textToSpeak)
{
	ISpVoice * ispVoice = NULL;
    if (FAILED(::CoInitialize(NULL)))
        return;

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&ispVoice);
    if (SUCCEEDED( hr )) {
	
		hr = ispVoice->Speak(_textToSpeak, SPF_ASYNC, NULL);
		if (hr == S_OK) {
			cout << "  - Function completed successfully." << endl;

		} else if (hr == E_INVALIDARG) {
			cout << "  - One or more parameters are invalid." << endl;

		} else if (hr == E_POINTER) {
			cout << "  - Invalid pointer." << endl;

		} else if (hr == E_OUTOFMEMORY) {
			cout << "  - Exceeded available memory." << endl;

		} else {
			cout << "  - Unknown error." << endl << endl;
		}

		hr = ispVoice->WaitUntilDone(30000);

        ispVoice->Release();
        ispVoice = NULL;
    }

    ::CoUninitialize();
}

void Speech::SayB(bstr_t _textToSpeak)
{
	ISpVoice * ispVoice = NULL;
    if (FAILED(::CoInitialize(NULL)))
        return;

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&ispVoice);
    if (SUCCEEDED( hr )) {
	
		hr = ispVoice->Speak(_textToSpeak, SPF_ASYNC, NULL);
		if (hr == S_OK) {
			cout << "  - Function completed successfully." << endl;

		} else if (hr == E_INVALIDARG) {
			cout << "  - One or more parameters are invalid." << endl;

		} else if (hr == E_POINTER) {
			cout << "  - Invalid pointer." << endl;

		} else if (hr == E_OUTOFMEMORY) {
			cout << "  - Exceeded available memory." << endl;

		} else {
			cout << "  - Unknown error." << endl << endl;
		}

		hr = ispVoice->WaitUntilDone(30000);

        ispVoice->Release();
        ispVoice = NULL;
    }
    ::CoUninitialize();
}

void Speech::Say(string _textToSpeak)
{
	bstr_t object(_textToSpeak.c_str());
	SayB(object);
		
}

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