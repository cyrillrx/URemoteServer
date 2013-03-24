#pragma once

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
		std::string m_key_word;
	};
}
