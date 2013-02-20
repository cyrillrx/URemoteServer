#include "ComHelper.h"

#include <sapi.h>

ComHelper::ComHelper()
{
	HRESULT hr = CoInitialize(nullptr);
	checkResult(hr);
}

ComHelper::~ComHelper()
{
	CoUninitialize();
}

void ComHelper::checkResult(HRESULT result)
{
	if (result != S_OK) {	
		throw std::exception(getResultMessage(result).c_str());
	}
}

std::string ComHelper::getResultMessage(HRESULT result)
{
	std::string msg;

	switch(result)
	{
	case E_INVALIDARG:
		msg = "One or more arguments are invalids.";
		break;

	case E_ACCESSDENIED:
		msg = "Acces Denied.";
		break;

	case E_NOINTERFACE:
		msg = "Interface does not exist.";
		break;

	case E_NOTIMPL:
		msg = "Not implemented method.";
		break;

	case E_OUTOFMEMORY:
		msg = "Out of memory.";
		break;

	case E_POINTER:
		msg = "Invalid pointer.";
		break;

	case E_UNEXPECTED:
		msg = "Unexpecter error.";
		break;

	case E_FAIL:
		msg = "Failure";
		break;

	default:
		msg = "Unknown";
		break;
	}

	return "COM error : " + msg;
}