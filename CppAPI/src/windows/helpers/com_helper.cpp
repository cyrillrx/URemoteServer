#include "com_helper.h"

#include <sapi.h>

#include "exception\Exception.h"

com_handler::com_handler()
{
	HRESULT hr = ::CoInitialize(nullptr);
	com_helper::check_result("::com_handler", hr);
}

com_handler::~com_handler()
{
	::CoUninitialize();
}

void com_helper::check_result(const std::string& source, const HRESULT& result)
{
	if (result != S_OK) {
		throw Exception("ComException", source, get_message(result));
	}
}

std::string com_helper::get_message(const HRESULT& result)
{
	std::string msg;

	switch(result) {

	case E_INVALIDARG:
		return "One or more arguments are invalids.";

	case E_ACCESSDENIED:
		return "Acces Denied.";

	case E_NOINTERFACE:
		return "Interface does not exist.";

	case E_NOTIMPL:
		return "Not implemented method.";

	case E_OUTOFMEMORY:
		return "Out of memory.";

	case E_POINTER:
		return "Invalid pointer.";

	case E_UNEXPECTED:
		return "Unexpecter error.";

	case E_FAIL:
		return "Failure";

	default:
		return "Unknown : " + std::to_string(result);
	}
}