#pragma once

#include <string>
#include "logger.h"

class ComHandler
{
public:
	ComHandler(logger logger);
	~ComHandler();

private:
	logger m_log;
};

namespace ComHelper
{
	void checkResult(const std::string& source, const long& result);
	std::string getResultMessage(const long& result);
}