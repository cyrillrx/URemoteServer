#pragma once

#include "exception/Exception.h"

class tts_exception : public Exception
{
public:
	tts_exception(const std::string& source, const std::string& message) throw();
};
