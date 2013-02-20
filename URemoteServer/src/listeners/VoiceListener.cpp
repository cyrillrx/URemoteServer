#include "VoiceListener.h"

const ULONGLONG grammarId = 0;
const wchar_t* ruleName1 = L"ruleName1";

VoiceListener::VoiceListener()
{
	m_log = new Logger("VoiceListener.log");
	m_log->setLogSeverityConsole(Logger::SEVERITY_LVL_WARNING);
}


VoiceListener::~VoiceListener()
{
	if (m_log) {
		delete(m_log);
		m_log = nullptr;
	}
}

void VoiceListener::doStart()
{
	m_log->info("******************************************************");
	m_log->info("*****            VoiceListener Started           *****");
	m_log->info("******************************************************");

	m_continueToListen = true;

	std::string entry;
	while (m_continueToListen) {

		// TODO: code voice recognition
		listen();
		// TODO: Add message : "command" is not supported
		// TODO: support "Kill" command
	}
	m_log->debug("EXIT !");
}

void VoiceListener::listen()
{
	ComHelper helper;
	HRESULT hr;

	ISpRecognizer* recognizer;
	hr = CoCreateInstance(CLSID_SpSharedRecognizer, 
		nullptr, CLSCTX_ALL, IID_ISpRecognizer,
		reinterpret_cast<void**>(&recognizer));
	helper.checkResult(hr);

	ISpRecoContext* recoContext;
	hr = recognizer->CreateRecoContext(&recoContext);
	helper.checkResult(hr);

	m_log->info("VoiceListener::listen, You should start Windows Recognition.");
	m_log->info("VoiceListener::listen, Start talking !");

	// Disable context
	hr = recoContext->Pause(0);
	helper.checkResult(hr);

	ISpRecoGrammar* recoGrammar = initGrammar(helper, recoContext);

	/////////////////////////////////////////////////
	// TODO: Review this
	hr = recoContext->SetNotifyWin32Event();
	helper.checkResult(hr);

	HANDLE handleEvent;
	handleEvent = recoContext->GetNotifyEventHandle();
	if(handleEvent == INVALID_HANDLE_VALUE) {
		helper.checkResult(E_FAIL);
	}

	// Si ci-dessous non invoqu�e, le moteur de reconnaissance d�marre
	// normalement, mais nous ne sommes jamais notifi�s de rien
	ULONGLONG interest;
	interest = SPFEI(SPEI_RECOGNITION);
	hr = recoContext->SetInterest(interest, interest);
	helper.checkResult(hr);

	// Activate Grammar
	hr = recoGrammar->SetRuleState(ruleName1, 0, SPRS_ACTIVE);
	helper.checkResult(hr);

	// Enable context
	hr = recoContext->Resume(0);
	helper.checkResult(hr);

	// Wait for reco
	HANDLE handles[1];
	handles[0] = handleEvent;
	WaitForMultipleObjects(1, handles, FALSE, INFINITE);
	getText(helper, recoContext);

	m_log->info("VoiceListener::listen, Yes sir");
}

ISpRecoGrammar* VoiceListener::initGrammar(ComHelper& helper, ISpRecoContext* recoContext)
{
	HRESULT hr;
	SPSTATEHANDLE sate;

	ISpRecoGrammar* recoGrammar;
	hr = recoContext->CreateGrammar(grammarId, &recoGrammar);
	helper.checkResult(hr);

	WORD langId = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH);
	hr = recoGrammar->ResetGrammar(langId);
	helper.checkResult(hr);

	// Create a rule
	hr = recoGrammar->GetRule(ruleName1, 0, SPRAF_TopLevel | SPRAF_Active, true, &sate);
	helper.checkResult(hr);

	// Add a word
	hr = recoGrammar->AddWordTransition(sate, NULL, L"Eternity", L" ", SPWT_LEXICAL, 1, nullptr);
	helper.checkResult(hr);

	// Commit changes
	hr = recoGrammar->Commit(0);
	helper.checkResult(hr);

	return recoGrammar;
}

void VoiceListener::getText(ComHelper& helper, ISpRecoContext* reco_context)
{
	const ULONG maxEvents = 10;
	SPEVENT events[maxEvents];


	ULONG eventCount;
	HRESULT hr;
	hr = reco_context->GetEvents(maxEvents, events, &eventCount);

	// Warning hr equal S_FALSE if everything is OK 
	// but eventCount < requestedEventCount
	if(!(hr == S_OK || hr == S_FALSE)) {
		helper.checkResult(hr);
	}

	m_log->info("Nb events: " + std::to_string(eventCount));
	ISpRecoResult* recoResult;
	recoResult = reinterpret_cast<ISpRecoResult*>(events[0].lParam);

	wchar_t* text;
	hr = recoResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, FALSE, &text, NULL);
	helper.checkResult(hr);

	m_log->info("VoiceListener::getText, J'ai compris: ");
	//	+ std::string(text));
	CoTaskMemFree(text);
}