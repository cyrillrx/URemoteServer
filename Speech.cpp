#include "Speech.h"

void Speech::SayB(bstr_t _textToSpeak)
{
	ISpVoice * ispVoice = NULL;
    if (FAILED(::CoInitialize(NULL)))
        return;

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&ispVoice);
    if (SUCCEEDED( hr )) {

		ispVoice->SetRate(-2);
	
		hr = ispVoice->Speak(_textToSpeak, SPF_ASYNC, NULL);
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
        ispVoice = NULL;
    }
    ::CoUninitialize();
}

void Speech::Say(string _textToSpeak)
{
	bstr_t object(_textToSpeak.c_str());
	SayB(object);
		
}