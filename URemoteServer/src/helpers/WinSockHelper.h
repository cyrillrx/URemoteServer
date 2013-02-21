#pragma once

#include <string>
#include <basetsd.h>

class WinSockHandler
{
public:
	WinSockHandler();
	~WinSockHandler();
};

class SocketHandler
{
public:
	SocketHandler(UINT_PTR socket);
	SocketHandler();
	~SocketHandler();

	UINT_PTR get();
	void set(UINT_PTR socket);

private:
	UINT_PTR m_socket;
};

namespace WinSockHelper
{
	void checkResult(const std::string& source, const int& result);
	std::string getResultMessage(const int& result);
}