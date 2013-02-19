#include "VoiceListener.h"


VoiceListener::VoiceListener(void)
{
	m_log = new Logger("ConsoleListener.log");
	m_log->setLogSeverityConsole(Logger::SEVERITY_LVL_WARNING);
}


VoiceListener::~VoiceListener(void)
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

		// TODO: Add message : "command" is not supported
		// TODO: support "Kill" command
	}
	m_log->debug("EXIT !");
}