#include "vr_exception.h"

vr_exception::vr_exception(const std::string& source, const std::string& message) throw()
	: Exception("vr_exception", source, message) { }
