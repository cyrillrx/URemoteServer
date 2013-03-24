#include "tts_exception.h"

tts_exception::tts_exception(const std::string& source, const std::string& message) throw()
	: Exception("tts_exception", source, message) { }
