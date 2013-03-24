#pragma once

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
	std::string get_message(const long& result);
}