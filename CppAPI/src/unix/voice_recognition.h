#pragma once

#include <string>

#include "logger.h"

namespace voice_recognition
{
	class recognizer
	{
	public:
		recognizer();
		~recognizer();

		void add_vocal_command();
		void start_listening(logger log);

	private:
		std::string m_key_word;
	};
}
