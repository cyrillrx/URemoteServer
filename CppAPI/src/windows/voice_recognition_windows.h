#pragma once

#include <string>
#include <sapi.h>

namespace voice_recognition
{
	class recognizer
	{
	public:
		recognizer();
		~recognizer();

		void add_vocal_command();
		void start_listening();

	private:
		std::string m_key_word;

		ISpRecoGrammar* initGrammar(ISpRecoContext* recoContext, const std::string& command);
		void getText(ISpRecoContext* reco_context);
	};
}