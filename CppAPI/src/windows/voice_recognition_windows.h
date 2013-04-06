#pragma once

#include "platform_config.h"
#if defined(WINDOWS_PLATFORM)

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
		std::string key_word_;

		ISpRecoGrammar* init_grammar(ISpRecoContext* reco_context, const std::string& command);
		void get_text(ISpRecoContext* reco_context);
	};
}

#endif
