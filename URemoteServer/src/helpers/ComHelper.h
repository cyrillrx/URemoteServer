#pragma once

#include <string>
#include "Logger.h"

class ComHandler
{
public:
	ComHandler(Logger* logger);
	~ComHandler();

private:
	Logger* m_log;
};

namespace ComHelper
{
	void checkResult(const std::string& source, const long& result);
	std::string getResultMessage(const long& result);
}