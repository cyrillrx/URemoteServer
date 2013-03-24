#pragma once

#include "exception/Exception.h"

class socket_exception : public Exception
{
public:
	socket_exception(const std::string& source, const std::string& message) throw();
};
