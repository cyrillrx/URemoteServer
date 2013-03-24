#pragma once

#include "exception/Exception.h"

class vr_exception : public Exception
{
public:
	vr_exception(const std::string& source, const std::string& message) throw();
};
