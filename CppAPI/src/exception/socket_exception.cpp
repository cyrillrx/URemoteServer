#include "socket_exception.h"

socket_exception::socket_exception(const std::string& source, const std::string& message) throw()
	: Exception("socket_exception", source, message) { }
