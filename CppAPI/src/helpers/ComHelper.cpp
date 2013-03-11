#include "ComHelper.h"

#include <sapi.h>

#include "exception\Exception.h"

ComHandler::ComHandler(logger logger)
	: m_log(logger)
{
	m_log.debug("Initializing COM library...");

	HRESULT hr = ::CoInitialize(nullptr);
	ComHelper::checkResult("::ComHandler", hr);
}

ComHandler::~ComHandler()
{
	m_log.debug("Uninitialize COM library.");

	::CoUninitialize();
}

namespace ComHelper
{
	void checkResult(const std::string& source, const HRESULT& result)
	{
		if (result != S_OK) {	
			throw Exception("ComException", source, getResultMessage(result));
		}
	}

	std::string getResultMessage(const HRESULT& result)
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
}