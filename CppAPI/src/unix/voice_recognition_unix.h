#pragma once

#include "platform_config.h"
#if defined(UNIX_PLATFORM)

#include <string>

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
	};
}

#endif
