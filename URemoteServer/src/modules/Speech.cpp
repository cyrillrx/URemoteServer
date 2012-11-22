#include "Speech.h"
#include <Windows.h>

void Speech::SayB(bstr_t _textToSpeak)
{
	ISpVoice * ispVoice = nullptr;
    if (FAILED(::CoInitialize(nullptr)))
        return;

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **)&ispVoice);
    if (SUCCEEDED( hr )) {

		ispVoice->SetRate(-2);
	
		hr = ispVoice->Speak(_textToSpeak, SPF_ASYNC, nullptr);
		if (hr == S_OK) {
			cout << "Speech::SayB : " << _textToSpeak << endl;

		} else if (hr == E_INVALIDARG) {
			cerr << "Speech::SayB - One or more parameters are invalid." << endl;

		} else if (hr == E_POINTER) {
			cerr << "Speech::SayB - Invalid pointer." << endl;

		} else if (hr == E_OUTOFMEMORY) {
			cerr << "Speech::SayB - Exceeded available memory." << endl;

		} else {
			cerr << "Speech::SayB - Unknown error." << endl << endl;
		}

		hr = ispVoice->WaitUntilDone(30000);

        ispVoice->Release();
        ispVoice = nullptr;
    }
    ::CoUninitialize();
}


DWORD WINAPI  Speech::SayThread(void* _textToSpeak)
{
	string *txt = (string*)_textToSpeak;
	Say(*txt);	

	delete(txt);
	return 0;
}

void Speech::Say(string _textToSpeak)
{
	bstr_t object(_textToSpeak.c_str());
	SayB(object);
		
}

void Speech::SayInThread(string _textToSpeak)
{
	string *ptTextToSpeak = new string(_textToSpeak);
	DWORD threadID;
	CreateThread(0, 0, SayThread, ptTextToSpeak, 0, &threadID);
}