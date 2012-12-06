#include "Speech.h"

#include <thread>

using namespace std;
Speech::Speech(const string& lang, const string& gender) : m_Language(lang), m_Gender(gender)
{
}

Speech::~Speech()
{
}

void Speech::initVoice(ISpVoice * ispVoice, const string& language, const string& gender)
{
	// TODO initialize Language and gender in a map
	const WCHAR* reqAttributs = (language == "FR")	? L"Language=40C" : L"Language=409";
	const WCHAR* optAttributs = (gender == "M")		? L"Gender=Male"  : L"Gender=Female";

	ISpObjectToken* cpTokenEng;
	HRESULT hr = SpFindBestToken(SPCAT_VOICES, reqAttributs, optAttributs, &cpTokenEng);
	ispVoice->SetVoice(cpTokenEng);
	ispVoice->SetRate(+1);
}

void Speech::SayB(const bstr_t& _textToSpeak, const std::string& language, const std::string& gender)
{
	ISpVoice * ispVoice = nullptr;
    if (FAILED(::CoInitialize(nullptr))) {
        return;
	}

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **)&ispVoice);
    if (SUCCEEDED( hr )) {

		initVoice(ispVoice, language, gender);
	
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

void Speech::Say(const string& textToSpeak, const std::string& language, const std::string& gender)
{
	bstr_t bstrTextToSpeak(textToSpeak.c_str());
	SayB(bstrTextToSpeak, language, gender);
		
}

void Speech::SayInThread(const string& textToSpeak)
{
	thread speakThread(Say, textToSpeak, m_Language, m_Gender);
	speakThread.join();
}