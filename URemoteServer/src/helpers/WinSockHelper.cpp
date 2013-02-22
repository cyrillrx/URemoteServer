#include "WinSockHelper.h"

#include <WinSock2.h>
#include "exception\Exception.h"

//////////////////////////////////////////////////////////////////////////////
// WinSockHandler
//////////////////////////////////////////////////////////////////////////////
WinSockHandler::WinSockHandler(Logger* logger)
	: m_log(logger)
{
	m_log->debug("Initializing winSock library (v2.0)...");

	WSADATA wsaData;
	::WSAStartup(MAKEWORD(2,0), &wsaData);
	WinSockHelper::checkResult("::WinSockHandler", WSAGetLastError());
}

WinSockHandler::~WinSockHandler()
{
	m_log->debug("Uninitialize WinSock library.");

	::WSACleanup();
	WinSockHelper::checkResult("::~WinSockHandler", WSAGetLastError());
}

//////////////////////////////////////////////////////////////////////////////
// SocketHandler
//////////////////////////////////////////////////////////////////////////////
SocketHandler::SocketHandler(SOCKET socket)
	: m_socket(socket) { }

SocketHandler::SocketHandler() { }

SocketHandler::~SocketHandler()
{
	closesocket(m_socket);
}

SOCKET SocketHandler::get()
{
	return m_socket;
}

void SocketHandler::set(SOCKET socket)
{
	m_socket = std::move(socket);
}

//////////////////////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////////////////////
namespace WinSockHelper
{
	void checkResult(const std::string& source, const int& result)
	{
		if (result != S_OK) {	
			throw Exception("WinSockException", source, getResultMessage(result));
		}
	}

	// see http://msdn.microsoft.com/en-us/library/windows/desktop/ms740668(v=vs.85).aspx
	std::string getResultMessage(const int& result)
	{
		std::string msg;

		switch(result) {

		case SOCKET_ERROR:
			return "Socket error";

		case WSANOTINITIALISED:
			return "Successful WSAStartup not yet performed.";

		case WSAEACCES:
			return "An attempt was made to access a socket in a way forbidden by its access permissions.";

		case WSAEFAULT:
			return "Bad address.";

		case WSAEINVAL:
			return "Invalid argument.";

		case WSAEWOULDBLOCK:
			return "Resource temporarily unavailable.";

		case WSAEALREADY:
			return "Operation already in progress.";

		case WSAEPROTONOSUPPORT:
			return "Protocol not supported.";

		case WSAESOCKTNOSUPPORT:
			return "Socket type not supported.";

		case WSAEAFNOSUPPORT:
			return "Address family not supported by protocol family.";

		default:
			return "Unknown : " + std::to_string(result);
		}
	}
}