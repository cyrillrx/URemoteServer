#include "platform_config.h"
#if defined(WINDOWS_PLATFORM)

#include "../voice_recognition.h"

#include <memory>
#include "Utils.h"
#include "helpers/com_helper.h"

const ULONGLONG grammarId = 0;
const wchar_t* ruleName1 = L"ruleName1";

voice_recognition::recognizer::recognizer() { }

voice_recognition::recognizer::~recognizer() { }

void voice_recognition::recognizer::add_vocal_command()
{
	// TODO: Implement recognizer::add_vocal_command()
}

void voice_recognition::recognizer::start_listening()
{
	// Initialize COM library
	com_handler initLibrary;
	HRESULT hr;

	ISpRecognizer* recognizer;
	hr = CoCreateInstance(CLSID_SpSharedRecognizer,
		nullptr, CLSCTX_ALL, IID_ISpRecognizer,
		reinterpret_cast<void**>(&recognizer));
	com_helper::check_result("VoiceListener::listen", hr);

	ISpRecoContext* recoContext;
	hr = recognizer->CreateRecoContext(&recoContext);
	com_helper::check_result("VoiceListener::listen", hr);

	Utils::get_logger()->info("VoiceListener::listen, You should start Windows Recognition.");
	Utils::get_logger()->info("VoiceListener::listen, Start talking !");

	// Disable context
	hr = recoContext->Pause(0);
	com_helper::check_result("VoiceListener::listen", hr);

	// TODO: Change static text to a parameter
	ISpRecoGrammar* recoGrammar = init_grammar(recoContext, "PC");

	/////////////////////////////////////////////////
	// TODO: Review this
	hr = recoContext->SetNotifyWin32Event();
	com_helper::check_result("VoiceListener::listen", hr);

	HANDLE handleEvent;
	handleEvent = recoContext->GetNotifyEventHandle();
	if(handleEvent == INVALID_HANDLE_VALUE) {
		com_helper::check_result("VoiceListener::listen", E_FAIL);
	}

	// Si ci-dessous non invoquée, le moteur de reconnaissance démarre
	// normalement, mais nous ne sommes jamais notifiés de rien
	ULONGLONG interest;
	interest = SPFEI(SPEI_RECOGNITION);
	hr = recoContext->SetInterest(interest, interest);
	com_helper::check_result("VoiceListener::listen", hr);

	// Activate Grammar
	hr = recoGrammar->SetRuleState(ruleName1, 0, SPRS_ACTIVE);
	com_helper::check_result("VoiceListener::listen", hr);

	// Enable context
	hr = recoContext->Resume(0);
	com_helper::check_result("VoiceListener::listen", hr);

	// Wait for reco
	HANDLE handles[1];
	handles[0] = handleEvent;
	WaitForMultipleObjects(1, handles, FALSE, INFINITE);
	get_text(recoContext);

	//TODO: implement callback
	//m_ai->say(lexicon_manager::getString(trad_key::AI_YES));
	Utils::get_logger()->info("AI_YES");

}

/**
* Create and initialize the Grammar.
* Create a rule for the grammar.
* Add word to the grammar.
*/
ISpRecoGrammar* voice_recognition::recognizer::init_grammar(ISpRecoContext* recoContext, const std::string& command)
{
	HRESULT hr;
	SPSTATEHANDLE sate;

	ISpRecoGrammar* recoGrammar;
	hr = recoContext->CreateGrammar(grammarId, &recoGrammar);
	com_helper::check_result("VoiceListener::initGrammar", hr);

	WORD langId = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH);
	hr = recoGrammar->ResetGrammar(langId);
	com_helper::check_result("VoiceListener::initGrammar", hr);
	// TODO: Catch error and use default langId => GetUserDefaultUILanguage()

	// Create rules
	hr = recoGrammar->GetRule(ruleName1, 0, SPRAF_TopLevel | SPRAF_Active, true, &sate);
	com_helper::check_result("VoiceListener::initGrammar", hr);

	// Add a word
	const std::wstring commandWstr = std::wstring(command.begin(), command.end());
	hr = recoGrammar->AddWordTransition(sate, NULL, commandWstr.c_str(), L" ", SPWT_LEXICAL, 1, nullptr);
	com_helper::check_result("VoiceListener::initGrammar", hr);

	// Commit changes
	hr = recoGrammar->Commit(0);
	com_helper::check_result("VoiceListener::initGrammar", hr);

	return recoGrammar;
}

void voice_recognition::recognizer::get_text(ISpRecoContext* reco_context)
{
	const ULONG maxEvents = 10;
	SPEVENT events[maxEvents];

	ULONG eventCount;
	HRESULT hr;
	hr = reco_context->GetEvents(maxEvents, events, &eventCount);

	// Warning hr equal S_FALSE if everything is OK 
	// but eventCount < requestedEventCount
	if(!(hr == S_OK || hr == S_FALSE)) {
		com_helper::check_result("VoiceListener::getText", hr);
	}

	ISpRecoResult* recoResult;
	recoResult = reinterpret_cast<ISpRecoResult*>(events[0].lParam);

	wchar_t* text;
	hr = recoResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, FALSE, &text, NULL);
	com_helper::check_result("VoiceListener::getText", hr);

	const std::wstring wstr = std::wstring(text);
	const std::string str(wstr.begin(), wstr.end());
	CoTaskMemFree(text);
}

#endif