#pragma once

#include "platform_config.h"
#if defined(WINDOWS_PLATFORM)

#include <string>

class com_handler
{
public:
	com_handler();
	~com_handler();
};

namespace com_helper
{
	void check_result(const std::string& source, const long& result);
	const std::string get_message(const long& result);
}

#endif
