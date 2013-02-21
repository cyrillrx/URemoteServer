#include "VoiceListener.h"

#include "..\helpers\ComHelper.h"
#include "..\AI.h"

const ULONGLONG grammarId = 0;
const wchar_t* ruleName1 = L"ruleName1";

VoiceListener::VoiceListener(AI *ai)
	: m_ai(ai)
{
	m_log = new Logger("VoiceListener.log");
	m_log->setLogSeverityConsole(Logger::SEVERITY_LVL_WARNING);
	
	//::CoInitialize(nullptr);
}


VoiceListener::~VoiceListener()
{
	//::CoUninitialize();
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
	// Initialize COM library
	m_log->debug("Initializing COM library...");
	ComHandler initLibrary;
	HRESULT hr;

	ISpRecognizer* recognizer;
	hr = CoCreateInstance(CLSID_SpSharedRecognizer,
		nullptr, CLSCTX_ALL, IID_ISpRecognizer,
		reinterpret_cast<void**>(&recognizer));
	ComHelper::checkResult("VoiceListener::listen", hr);

	ISpRecoContext* recoContext;
	hr = recognizer->CreateRecoContext(&recoContext);
	ComHelper::checkResult("VoiceListener::listen", hr);

	m_log->info("VoiceListener::listen, You should start Windows Recognition.");
	m_log->info("VoiceListener::listen, Start talking !");

	// Disable context
	hr = recoContext->Pause(0);
	ComHelper::checkResult("VoiceListener::listen", hr);

	ISpRecoGrammar* recoGrammar = initGrammar(recoContext);

	/////////////////////////////////////////////////
	// TODO: Review this
	hr = recoContext->SetNotifyWin32Event();
	ComHelper::checkResult("VoiceListener::listen", hr);

	HANDLE handleEvent;
	handleEvent = recoContext->GetNotifyEventHandle();
	if(handleEvent == INVALID_HANDLE_VALUE) {
		ComHelper::checkResult("VoiceListener::listen", E_FAIL);
	}

	// Si ci-dessous non invoquée, le moteur de reconnaissance démarre
	// normalement, mais nous ne sommes jamais notifiés de rien
	ULONGLONG interest;
	interest = SPFEI(SPEI_RECOGNITION);
	hr = recoContext->SetInterest(interest, interest);
	ComHelper::checkResult("VoiceListener::listen", hr);

	// Activate Grammar
	hr = recoGrammar->SetRuleState(ruleName1, 0, SPRS_ACTIVE);
	ComHelper::checkResult("VoiceListener::listen", hr);

	// Enable context
	hr = recoContext->Resume(0);
	ComHelper::checkResult("VoiceListener::listen", hr);

	// Wait for reco
	HANDLE handles[1];
	handles[0] = handleEvent;
	WaitForMultipleObjects(1, handles, FALSE, INFINITE);
	getText(recoContext);

	m_ai->say("Yes sir");

}

ISpRecoGrammar* VoiceListener::initGrammar(ISpRecoContext* recoContext)
{
	HRESULT hr;
	SPSTATEHANDLE sate;

	ISpRecoGrammar* recoGrammar;
	hr = recoContext->CreateGrammar(grammarId, &recoGrammar);
	ComHelper::checkResult("VoiceListener::initGrammar", hr);

	WORD langId = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH);
	hr = recoGrammar->ResetGrammar(langId);
	ComHelper::checkResult("VoiceListener::initGrammar", hr);

	// Create rules
	hr = recoGrammar->GetRule(ruleName1, 0, SPRAF_TopLevel | SPRAF_Active, true, &sate);
	ComHelper::checkResult("VoiceListener::initGrammar", hr);
	
	// Add a word
	const std::string aiName = m_ai->getName();
	const std::wstring aiNameWstr = std::wstring(aiName.begin(), aiName.end());
	const wchar_t* name = aiNameWstr.c_str();
	hr = recoGrammar->AddWordTransition(sate, NULL, name, L" ", SPWT_LEXICAL, 1, nullptr);
	ComHelper::checkResult("VoiceListener::initGrammar", hr);

	// Commit changes
	hr = recoGrammar->Commit(0);
	ComHelper::checkResult("VoiceListener::initGrammar", hr);

	return recoGrammar;
}

void VoiceListener::getText(ISpRecoContext* reco_context)
{
	const ULONG maxEvents = 10;
	SPEVENT events[maxEvents];

	ULONG eventCount;
	HRESULT hr;
	hr = reco_context->GetEvents(maxEvents, events, &eventCount);

	// Warning hr equal S_FALSE if everything is OK 
	// but eventCount < requestedEventCount
	if(!(hr == S_OK || hr == S_FALSE)) {
		ComHelper::checkResult("VoiceListener::getText", hr);
	}

	m_log->info("Nb events: " + std::to_string(eventCount));
	ISpRecoResult* recoResult;
	recoResult = reinterpret_cast<ISpRecoResult*>(events[0].lParam);

	wchar_t* text;
	hr = recoResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, FALSE, &text, NULL);
	ComHelper::checkResult("VoiceListener::getText", hr);

	const std::wstring wstr = std::wstring(text);
	const std::string str(wstr.begin(), wstr.end());
	m_log->info("VoiceListener::getText, J'ai compris: " + str);
	CoTaskMemFree(text);
}