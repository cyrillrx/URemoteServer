#pragma once

#include <string>

class ComHandler
{
public:
	ComHandler();
	~ComHandler();
};

namespace ComHelper
{
	void checkResult(const std::string& source, const long& result);
	std::string getResultMessage(const long& result);
}